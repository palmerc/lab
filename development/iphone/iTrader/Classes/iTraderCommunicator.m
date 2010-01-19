//
//  iTraderCommunicator.m
//  iTraderCommunicator is a Singleton that the rest of the application can use to
//  communicate with the mTraderServer
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

#import "iTraderCommunicator.h"
#import "UserDefaults.h";
#import "Symbol.h"
#import "Feed.h"
#import "Chart.h"

@implementation iTraderCommunicator

static iTraderCommunicator *sharedCommunicator = nil;
@synthesize mTraderServerDataDelegate;
@synthesize stockAddDelegate;
@synthesize isLoggedIn;
@synthesize communicator = _communicator;
@synthesize defaults = _defaults;
@synthesize blockBuffer = _blockBuffer;
@synthesize currentLine = _currentLine;
@synthesize state;

#pragma mark Initialization, and Cleanup
/**
 * Basic object setup and tear down
 *
 */
- (id)init {
	self = [super init];
	if (self != nil) {
		self.communicator = [[Communicator alloc] initWithSocket:@"wireless.theonlinetrader.com" port:7780];
		self.communicator.delegate = self;
		self.defaults = [UserDefaults sharedManager];
		
		isLoggedIn = NO;
		loginStatusHasChanged = NO;
		_blockBuffer = [[NSMutableArray alloc] init];
		contentLength = 0;
		state = LOGIN;
	}
	return self;
}

- (void)dealloc {
	[self logout];
	[self.blockBuffer release];
	[self.communicator stopConnection];
	[self.communicator release];
	[super dealloc];
}

#pragma mark Singleton Methods
/**
 * Methods for Singleton implementation
 *
 */
+ (iTraderCommunicator *)sharedManager {
	if (sharedCommunicator == nil) {
		sharedCommunicator = [[super allocWithZone:NULL] init];
	}
	return sharedCommunicator;
}

+ (id)allocWithZone:(NSZone *)zone {
	return [[self sharedManager] retain];
}

- (id)copyWithZone:(NSZone *)zone {
	return self;
}

- (id)retain {
	return self;
}

- (NSUInteger)retainCount {
	return NSUIntegerMax;
}

- (void)release {
	// do nothing
}

- (id)autorelease {
	return self;
}



#pragma mark Data Received
- (void)dataReceived {
	self.currentLine = [self.communicator readLine];
	NSLog(@"%@", [self currentLineToString]);
	
	switch (state) {
		case CHART:
			[self chartHandling];
			break;
		case CONTENTLENGTH:
			[self contentLength];
			break;
		case LOGIN:
			[self loginHandling];
			break;
		case PREPROCESSING:
			[self preprocessing];
			break;
		case PROCESSING:
			[self processingLoop];
			break;
		case QUOTE:
			[self quoteHandling];
			break;
		case STATIC:
			[self staticLoop];
			break;
		case ADDSEC:
			[self addSecurityOK];
			break;
		case REMSEC:
			[self removeSecurityOK];
			break;
		case STATDATA:
			[self staticDataOK];
			break;
		default:
			NSLog(@"Invalid state: %d", state);
			break;
	}
}

#pragma mark State Machine
/**
 * State machine methods called from -(void)dataReceived
 *
 */
- (void)contentLength {
	assert(contentLength > 0);
	[self.blockBuffer addObject:self.currentLine];
	contentLength -= [self.currentLine length];	
	if (contentLength == 0) {
		state = PROCESSING;
		// Call someone to deal with this
		// Wipe out block buffer
	}
}

- (void)loginHandling {
	NSString *line = [self currentLineToString];
	if ([line rangeOfString:@"Request: login/OK"].location == 0) {
		loginStatusHasChanged = YES;
		isLoggedIn = YES;
		state = PREPROCESSING;
	} else if ([line rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		loginStatusHasChanged = YES;
		isLoggedIn = NO;
	}
}

- (void)preprocessing {
	[self.blockBuffer addObject:self.currentLine];
	NSString *line = [self currentLineToString];
	if ([line rangeOfString:@"Quotes:"].location == 0) {
		[self settingsParsing];
		state = PROCESSING;
	}
}

- (void)processingLoop {
	[self.blockBuffer addObject:self.currentLine];
	NSString *line = [self currentLineToString];
	if ([line rangeOfString:@"HTTP/1.1 200 OK"].location == 0) { // Static data
		state = STATIC;
	} else if ([line rangeOfString:@"Request: q"].location == 0) { // Streaming
		state = QUOTE;
	}
}

- (void)staticLoop {
	[self.blockBuffer addObject:self.currentLine];
	NSString *line = [self currentLineToString];
	if ([line rangeOfString:@"Request: q"].location == 0) {
		state = QUOTE;
	} else if ([line rangeOfString:@"Content-Length: "].location == 0) {
		state = CONTENTLENGTH;
		NSRange contentLengthRange = [line rangeOfString:@"Content-Length: "];
		NSRange size;
		size.location = contentLengthRange.length;
		size.length = [line length] - contentLengthRange.length;
		contentLength = [[line substringWithRange:size] integerValue] + 2;		
	} else if 	 ([line rangeOfString:@"Request: Chart/OK"].location == 0) {
		state = CHART;
	} else if  ([line rangeOfString:@"Request: addSec/OK"].location == 0) {
		state = ADDSEC;
	} else if  ([line rangeOfString:@"Request: remSec/OK"].location == 0) {
		state = REMSEC;
	} else if ([line rangeOfString:@"Request: StaticData/OK"].location == 0) {
		state = STATDATA;
	} else if ([line rangeOfString:@"Request: addSec/failed.NoSuchSec"].location == 0) {
		[stockAddDelegate addFailedNotFound];
		state = PROCESSING;
	} else if 	 ([line rangeOfString:@"Request: addSec/failed.AlreadyExists"].location == 0) {
		[stockAddDelegate addFailedAlreadyExists];
		state = PROCESSING;
	} else if ([line rangeOfString:@"Request: remSec/CouldNotDelete"].location == 0) {
		state = PROCESSING;
	} else if ([line rangeOfString:@"Server:"].location == 0) {
		
	} else if ([line isEqualToString:@""]) {			
		// Ignore blank lines
	} else {
		NSLog(@"Invalid state: %d", state);
	}
}

- (void)quoteHandling {
	[self.blockBuffer addObject:self.currentLine];
	NSString *line = [self currentLineToString];
	
	if ([line rangeOfString:@"Quotes:"].location == 0) {
		NSArray *quotes = [self quotesParsing:line];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
			[self.mTraderServerDataDelegate updateQuotes:quotes];
		}
		state = PROCESSING;
	} else if ([line rangeOfString:@"Kickout: 1"].location == 0) {
//		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Kickout" message:@"You have been logged off since you logged in from another client" delegate:self cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
//		[alertView show];
//		[alertView release];
		state = KICKOUT;
		// Tell the user they are no longer logged on and how to correct it.		
	} else {
		NSLog(@"Keep-Alive");
		state = PROCESSING;
	}
}

- (void)chartHandling {
	[self.blockBuffer addObject:self.currentLine];
	
	NSString *line = [self currentLineToString];
	if ([line rangeOfString:@"<ImageEnd>"].location != NSNotFound) {
		Chart *chart = [self chartParsing];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(chart:)]) {
			[self.mTraderServerDataDelegate chart:chart];
		}
		state = PROCESSING;
	}
}

- (void)addSecurityOK {
	[self.blockBuffer addObject:self.currentLine];
	NSString *line = [self currentLineToString];
	if ([line rangeOfString:@"SecInfo:"].location == 0) {
		[stockAddDelegate addOK];
		NSArray *quotes = [self quotesParsing:line];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
			[self.mTraderServerDataDelegate updateQuotes:quotes];
		}
		state = PROCESSING;
	}
}

- (void)removeSecurityOK {
	// Unimplemented
}

- (void)staticDataOK {
	[self.blockBuffer addObject:self.currentLine];

	NSString *line = [self currentLineToString];
	if ([line rangeOfString:@"SecOid:"].location == 0) {
		// Just save it
	} else if ([line rangeOfString:@"Staticdata: "].location == 0) {
		[self staticDataParsing];
		state = PROCESSING;
	}
}

#pragma mark Parsing
/**
 * These are methods that parse blocks of data
 *
 */
- (void)settingsParsing {
	for (NSData *data in self.blockBuffer) {
		NSString *line = [self dataToString:data];
		if ([line rangeOfString:@"Symbols:"].location == 0) {
			[self symbolsParsing:line];
		} else if ([line rangeOfString:@"Quotes:"].location == 0) {
			NSArray *quotes = [self quotesParsing:line];
			if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
				[self.mTraderServerDataDelegate updateQuotes:quotes];
			}
			state = PROCESSING;
			break;
		}
	}
}

- (void)staticDataParsing {
	NSMutableDictionary *dataDictionary = [[NSMutableDictionary alloc] init];
	for (NSData *data in self.blockBuffer) {
		NSString *line = [self dataToString:data];
		if ([line rangeOfString:@"SecOid:"].location == 0) {
			NSString *feedTicker = [self cleanString:[[line componentsSeparatedByString:@":"] objectAtIndex:1]];
			[dataDictionary setObject:feedTicker forKey:@"feedTicker"];
		} else if ([line rangeOfString:@"Staticdata:"].location == 0) {
			NSRange staticDataRange = [line rangeOfString:@"Staticdata: "];
			NSRange restOfTheDataRange;
			restOfTheDataRange.location = staticDataRange.length;
			restOfTheDataRange.length = [line length] - staticDataRange.length;
			NSString *staticDataString = [line substringWithRange:restOfTheDataRange];
			
			
			NSArray *staticDataRows = [staticDataString componentsSeparatedByString:@";"];
			for (NSString *row in staticDataRows) {
				NSRange separatorRange = [row rangeOfString:@":"];
				if (separatorRange.location != NSNotFound) {
					NSString *key = [row substringToIndex:separatorRange.location];
					NSString *value = [row substringFromIndex:separatorRange.location + 1];
					[dataDictionary setObject:value forKey:key];
				}
				
			}
		}
	}
	if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(staticUpdates:)]) {
		[self.mTraderServerDataDelegate staticUpdates:dataDictionary];
	}
	state = PROCESSING;
	[dataDictionary release];
}

/**
 * We expect the block buffer holds a valid chart in its entirety.
 * Each line should be NSData from Http header to ImageEnd
 */
- (Chart *)chartParsing {
	
	Chart *chart = [[Chart alloc] init];
	
	// Get the lines that are strings
	int numberOfItemsToDelete = 0;
	NSString *imageSize = nil;
	for (NSData *data in self.blockBuffer) {
		NSString *line = [self dataToString:data];
		if ([line rangeOfString:@"<ImageBegin>"].location == NSNotFound) {
			NSArray *partsOfString = [line componentsSeparatedByString:@":"];
			if ([partsOfString count] > 1) {
				NSString *dataPortion = [[self stripOffFirstElement:partsOfString] objectAtIndex:0];
				NSString *cleanedDataPortion = [self cleanString:dataPortion];
				if ([line rangeOfString:@"SecOid:"].location == 0) {
					chart.feedTicker = cleanedDataPortion;
				} else if ([line rangeOfString:@"Width:"].location == 0) {
					chart.width = [cleanedDataPortion integerValue];
				} else if ([line rangeOfString:@"Height:"].location == 0) {
					chart.height = [cleanedDataPortion integerValue];
				} else if ([line rangeOfString:@"ImgType:"].location == 0) {
					chart.imageType = cleanedDataPortion;
				} else if ([line rangeOfString:@"ImageSize:"].location == 0) {
					imageSize = cleanedDataPortion; // retain?
					chart.size = [cleanedDataPortion integerValue];
				}
			}
			numberOfItemsToDelete++;
		} else {
			break;
		}
	}
	
	NSRange range;
	range.location = 0;
	range.length = numberOfItemsToDelete;
	NSIndexSet *indexes = [NSIndexSet indexSetWithIndexesInRange:range];
	[self.blockBuffer removeObjectsAtIndexes:indexes];
	
	numberOfItemsToDelete = 0;
	NSMutableData *imageData = [[NSMutableData alloc] init];
	
	BOOL begin = NO;
	BOOL end = NO;
	for (NSData *data in self.blockBuffer) {
		NSString *line = [self dataToString:data];
		if ([line rangeOfString:@"<ImageBegin>"].location != NSNotFound) {
			NSRange imageBegin = [line rangeOfString:@"<ImageBegin>"];
			NSUInteger length = imageBegin.length;
			imageBegin.location = length;
			imageBegin.length = [data length] - length;
			data = [data subdataWithRange:imageBegin];
			line = [self dataToString:data];
			
			begin = YES;
		} 
		
		if ([line rangeOfString:@"<ImageEnd>"].location != NSNotFound) {
			NSRange imageEnd = [line rangeOfString:@"<ImageEnd>"];
			imageEnd.length = imageEnd.location;
			imageEnd.location = 0;
			data = [data subdataWithRange:imageEnd];
			
			end = YES;
		}
		
		[imageData appendData:data];
		numberOfItemsToDelete++;
	}
		
	range.location = 0;
	range.length = numberOfItemsToDelete;
	indexes = [NSIndexSet indexSetWithIndexesInRange:range];
	[self.blockBuffer removeObjectsAtIndexes:indexes];
	
	if ([imageData length] != chart.size) {
		NSException *exception = [NSException exceptionWithName:@"ImageSizeMismatch" 
														 reason:@"ImageSize specified didn't match received data size." 
													   userInfo:[NSDictionary dictionaryWithObjectsAndKeys:imageSize, @"ImageSize", imageData, @"ImageData", nil]];
		[exception raise];
	}
	chart.image = imageData;
	[imageData release];
	
	if (!begin || !end) {
		[chart release];
		return nil;
	} else {	
		return chart;
	}
}

- (NSArray *)quotesParsing:(NSString *)quotes {
	NSArray *quotesAndTheRest = [self stripOffFirstElement:[quotes componentsSeparatedByString:@":"]];
	NSString *theRest = [quotesAndTheRest objectAtIndex:0];
	return [theRest componentsSeparatedByString:@"|"];
}

- (void)symbolsParsing:(NSString *)symbols {
	// remove the part of the string preceding the colon, and the rest of the symbols are colon separated.
	NSArray *rows = [self stripOffFirstElement:[symbols componentsSeparatedByString:@":"]];
	
	// For each symbol
	for (NSString *row in rows) {
		if (![row isEqualToString:@""]) {
			Symbol *symbol = [[Symbol alloc] init];
			
			// The symbol data is separated by semi-colons.
			NSArray *columns = [self cleanStrings:[row componentsSeparatedByString:@";"]];
			
			// Clean up the white space.
			symbol.feedTicker = [columns objectAtIndex:0];
			
			// Split the feedNumberAndTicker into two components
			NSArray *feedNumberAndTicker = [symbol.feedTicker componentsSeparatedByString:@"/"];
			symbol.feedNumber = [feedNumberAndTicker objectAtIndex:0];
			NSString *tickerToo = [feedNumberAndTicker objectAtIndex:1];
			NSString *ticker = [columns objectAtIndex:1];
			// The ticker symbol from field 0 should match the same symbol in field 1
			assert([ticker isEqualToString:tickerToo]);
			symbol.tickerSymbol = ticker;
			
			symbol.name = [columns objectAtIndex:2];
			NSString *feedDescriptionAndCode = [columns objectAtIndex:3];
			symbol.type = [columns objectAtIndex:4];
			symbol.orderbook = [columns objectAtIndex:5];
			symbol.isin = [columns objectAtIndex:6];
			symbol.exchangeCode = [columns objectAtIndex:7];
			
			Feed *feed = [[Feed alloc] init];
			feed.feedNumber = symbol.feedNumber;
			// Obj-C doesn't have regex so we will look from the end of the string to get the [xxx] feed code. 
			NSCharacterSet *leftSquareBracket = [NSCharacterSet characterSetWithCharactersInString:@"["]; 
			NSRange feedCodeRange = [feedDescriptionAndCode rangeOfCharacterFromSet:leftSquareBracket options:NSBackwardsSearch];
			NSInteger lengthOfFeedString = [feedDescriptionAndCode length];
			
			// Range compensating for the removal of square brackets
			NSRange codeRange;
			codeRange.location = feedCodeRange.location + 1;
			codeRange.length = (lengthOfFeedString - feedCodeRange.location) - 2;
			
			NSRange descriptionRange;
			descriptionRange.location = 0;
			descriptionRange.length = lengthOfFeedString - codeRange.length - 2;
			
			NSString *feedDescription = [feedDescriptionAndCode substringWithRange:descriptionRange];
			NSString *feedCode = [feedDescriptionAndCode substringWithRange:codeRange];
			feed.feedDescription = feedDescription;
			feed.code = feedCode;
			
			if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(addSymbol: withFeed:)]) {
				[self.mTraderServerDataDelegate addSymbol:symbol withFeed:feed];
			}
			[symbol release];
			[feed release];
			symbol = nil;
			feed = nil;
		}
	}
}


- (BOOL)loginStatusHasChanged {
	BOOL result = NO;
	if (loginStatusHasChanged) {
		result = YES;
		
		loginStatusHasChanged = NO;
	}
	return result;
}

#pragma mark mTrader Server Message Sending
/**
 * These are methods that format mTrader Server Messages
 *
 */

- (void)logout {
	NSString *ActionLogout = @"Action: Logout";
	NSArray *logoutArray = [NSArray arrayWithObjects:ActionLogout, nil];
	NSString *logoutString = [self arrayToFormattedString:logoutArray];
	[self.communicator writeString:logoutString];
}

- (void)login {
	NSString *username = self.defaults.username;
	NSString *password = self.defaults.password;
	
	if (username != nil && password != nil) {
		NSString *version = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
		NSString *build = [NSString stringWithFormat:@"%@",[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"]];
		
		NSString *ActionLogin = @"Action: login";
		NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@/%@", username, password];
		//NSString *Platform = [NSString stringWithFormat:@"Platform: %@ %@", [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion]];
		NSString *Client = @"Client: iTrader";
		NSString *Version = [NSString stringWithFormat:@"VerType: %@.%@", version, build];
		NSString *ConnectionType = @"ConnType: Socket";
		NSString *Streaming = @"Streaming: 1";
		NSString *QFields = @"QFields: l;cp;b;a;av;bv;c;h;lo;o;v";
		//NSString *QFields = @"QFields: l";
		
		NSArray *loginArray = [NSArray arrayWithObjects:ActionLogin, Authorization, Client, Version, ConnectionType, Streaming, QFields, nil];
		NSString *loginString = [self arrayToFormattedString:loginArray];
		
		if ([self.communicator isConnected]) {
			[self.communicator stopConnection];
			isLoggedIn = NO;
		}
		
		[self.communicator startConnection];
		[self.communicator writeString:loginString];
	}
}

- (void)staticDataForFeedTicker:(NSString *)feedTicker {
	NSString *username = self.defaults.username;
	NSString *language = @"EN";
	NSString *ActionStatData = @"Action: StatData";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	NSString *Language = [NSString stringWithFormat:@"Language: %@", language];
	
	NSArray *getStatDataArray = [NSArray arrayWithObjects:ActionStatData, Authorization, SecOid, Language, nil];
	NSString *statDataRequestString = [self arrayToFormattedString:getStatDataArray];
	
	[self.communicator writeString:statDataRequestString];
}

- (void)graphForFeedTicker:(NSString *)feedTicker period:(NSUInteger)period width:(NSUInteger)width height:(NSUInteger)height orientation:(NSString *)orientation {
	NSString *imgType = @"PNG"; // We only support one type of image currently although GIF is also specified in client.
	
	NSString *username = self.defaults.username;
	NSString *ActionChart = @"Action: Chart";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	NSString *Period = [NSString stringWithFormat:@"Period: %d", period];
	NSString *ImgType = [NSString stringWithFormat:@"ImgType: %@", imgType];
	NSString *Width = [NSString stringWithFormat:@"Width: %d", width];
	NSString *Height = [NSString stringWithFormat:@"Height: %d", height];
	NSString *Orient = [NSString stringWithFormat:@"Orient: %@", orientation]; // (A)uto, (H)orizontal, and (V)ertical
	
	NSArray *getChartArray = [NSArray arrayWithObjects:ActionChart, Authorization, SecOid, Period, ImgType, Width, Height, Orient, nil];
	NSString *getChartString = [self arrayToFormattedString:getChartArray];
	
	[self.communicator writeString:getChartString];
}

- (void)addSecurity:(NSString *)tickerSymbol {
	NSString *username = self.defaults.username;
	
	NSString *ActionAddSec = @"Action: addSec";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *Search = [NSString stringWithFormat:@"Search: %@", tickerSymbol];
	NSString *MCode = [NSString stringWithFormat:@"mCode: %@", @"Oslo Stocks [OSS]"];
	
	NSArray *addSecurityArray = [NSArray arrayWithObjects:ActionAddSec, Authorization, Search, MCode, nil];
	NSString *addSecurityString = [self arrayToFormattedString:addSecurityArray];
	
	[self.communicator writeString:addSecurityString];
}

- (void)removeSecurity:(NSString *)feedTicker {
	NSString *username = self.defaults.username;
	
	NSString *ActionRemSec = @"Action: remSec";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	
	NSArray *removeSecurityArray = [NSArray arrayWithObjects:ActionRemSec, Authorization, SecOid, nil];
	NSString *removeSecurityString = [self arrayToFormattedString:removeSecurityArray];
	
	[self.communicator writeString:removeSecurityString];
}

#pragma mark Helper Methods
/**
 * Helper methods
 *
 */

- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings {
	NSString *EOL = @"\r\n";
	
	NSMutableString *appendableText = [[NSMutableString alloc] init];
	for (NSString *string in arrayOfStrings) {
		NSString *current = [[NSString alloc] initWithFormat:@"%@%@", string, EOL];
		[appendableText appendString:current];
		[current release];
	}
	[appendableText appendString:EOL]; // A blank line indicates the end of the sending block
	NSString *immutableString = [NSString stringWithUTF8String:[appendableText UTF8String]];
	[appendableText release];
	
	return immutableString;
}

- (NSArray *)stripOffFirstElement:(NSArray *)array {
	NSRange rowsWithoutFirstString;
	rowsWithoutFirstString.location = 1;
	rowsWithoutFirstString.length = [array count] - 1;
	NSArray *rows = [array subarrayWithRange:rowsWithoutFirstString];
	
	return rows;
}

- (NSString *)dataToString:(NSData *)data {
	NSString *string = [[NSString alloc] initWithData:data encoding:NSISOLatin1StringEncoding];
	NSString *final = [NSString stringWithString:[self cleanString:string]];
	[string release];
	return final;
}

- (NSString *)currentLineToString {
	NSString *string = [[NSString alloc] initWithData:self.currentLine encoding:NSISOLatin1StringEncoding];
	NSString *final = [NSString stringWithString:[self cleanString:string]];
	[string release];
	return final;
}

- (NSString *)cleanString:(NSString *)string {
	NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
		
	NSString *aCleanString = [string stringByTrimmingCharactersInSet:whitespaceAndNewline];
	
	return aCleanString;
}

- (NSArray *)cleanStrings:(NSArray *)strings {
	NSMutableArray *mutableProduct = [[NSMutableArray alloc] init];
	
	for (NSString *string in strings) {
		[mutableProduct addObject:[self cleanString:string]];
	}
		
	NSArray *finalProduct = [NSArray arrayWithArray:mutableProduct];
	[mutableProduct release];
	
	return finalProduct;
}

@end
