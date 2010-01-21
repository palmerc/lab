//
//  iTraderCommunicator.m
//  iTraderCommunicator is a Singleton that the rest of the application can use to
//  communicate with the mTraderServer
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

#import "iTraderCommunicator.h"
#import "NSMutableArray+QueueAdditions.h"
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
@synthesize state, contentLength;

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
		self.blockBuffer = [[NSMutableArray alloc] init];
		contentLength = 0;
		state = HEADER;
	}
	return self;
}

- (void)dealloc {
	[self.blockBuffer release];
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

/**
 * My little lexer
 */
- (void)dataReceived {
	NSData *data = [self.communicator readLine];
	NSString *string = [self dataToString:data];
	
	if (![string isEqualToString:@"\r\r"]) {
		[self.blockBuffer addObject:data];
	} else {
		[self stateMachine];
		[self.blockBuffer removeAllObjects];
	}
}	
	
-(void) stateMachine {
	while ([self.blockBuffer count] > 0) {
		switch (state) {
			case HEADER:
				[self headerParsing];
				break;
			case FIXEDLENGTH:
				[self fixedLength];
				break;
			case STATICRESPONSE:
				[self staticResponse];
				break;
			case CHART:
				[self chartHandling];
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
}

#pragma mark State Machine
/**
 * State machine methods called from -(void)dataReceived
 *
 */


-(void) headerParsing {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	if ([string rangeOfString:@"Content-Length:"].location == 0) {
		NSString *rhs = [self cleanString:[self dataFromRHS:string]];
		contentLength = [rhs integerValue];
		state = FIXEDLENGTH;
	} else if ([string isEqual:@"\r\n"]) {
		state = STATICRESPONSE;
	}
}

-(void) fixedLength {
	[self.blockBuffer deQueue]; // blank line
	
	int bytes = 0;
	for (NSData *data in self.blockBuffer) {
		bytes += [data length];
	}
	bytes += 2; // For the CR CR we skipped at the end of the block.
	
	if (bytes == contentLength) {
		contentLength = 0;
	} else {
		NSLog(@"Fixed-length response didn't match advertised size: %d vs. %d", contentLength, bytes);
	}
	
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		loginStatusHasChanged = YES;
		isLoggedIn = NO;
		state = LOGIN;
	} else if ([string rangeOfString:@"Request: login/failed.DeniedAccess"].location == 0) {
		loginStatusHasChanged = YES;
		isLoggedIn = NO;
		state = LOGIN;
	} 
}

-(void) staticResponse {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"Request: login/OK"].location == 0) {
		loginStatusHasChanged = YES;
		isLoggedIn = YES;
		state = PREPROCESSING;
	} else if  ([string rangeOfString:@"Request: addSec/OK"].location == 0) {
		state = ADDSEC;
	} else if  ([string rangeOfString:@"Request: remSec/OK"].location == 0) {
		state = REMSEC;
	} else if ([string rangeOfString:@"Request: Chart/OK"].location == 0) {
		state = CHART;
	} else if ([string rangeOfString:@"Request: StaticData/OK"].location == 0) {
		state = STATDATA;
	} else if ([string rangeOfString:@"Request: addSec/failed.NoSuchSec"].location == 0) {
		[stockAddDelegate addFailedNotFound];
		state = PROCESSING;
	} else if 	 ([string rangeOfString:@"Request: addSec/failed.AlreadyExists"].location == 0) {
		[stockAddDelegate addFailedAlreadyExists];
		state = PROCESSING;
	} else if ([string rangeOfString:@"Request: q"].location == 0) {
		state = QUOTE;
	}
}

- (void)loginHandling {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"Request: login/OK"].location == 0) {
		loginStatusHasChanged = YES;
		isLoggedIn = YES;
		state = PREPROCESSING;
	} else if ([string rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		loginStatusHasChanged = YES;
		isLoggedIn = NO;
	}
}

/**
 * Read the login stream upto quotes, call settingsParsing to deal with it and
 * change state.
 */
- (void)preprocessing {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];

	if ([string rangeOfString:@"Symbols:"].location == 0) {
		[self symbolsParsing:string];
	} else if ([string rangeOfString:@"Quotes:"].location == 0) {
		NSArray *quotes = [self quotesParsing:string];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
			[self.mTraderServerDataDelegate updateQuotes:quotes];
		}
		state = PROCESSING;
	}
}

- (void)processingLoop {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"HTTP/1.1 200 OK"].location == 0) { // Static data
		state = HEADER;
	} else if ([string rangeOfString:@"Request: q"].location == 0) { // Streaming
		if ([self.blockBuffer count] > 0) {
			state = QUOTE;
		} else {
			// Keep-Alive
			state = PROCESSING;
		}
	}
}

- (void)quoteHandling {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"Quotes:"].location == 0) {
		NSArray *quotes = [self quotesParsing:string];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
			[self.mTraderServerDataDelegate updateQuotes:quotes];
		}
		state = PROCESSING;
	} else if ([string rangeOfString:@"Kickout: 1"].location == 0) {
//		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Kickout" message:@"You have been logged off since you logged in from another client" delegate:self cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
//		[alertView show];
//		[alertView release];
		state = KICKOUT;
		// Tell the user they are no longer logged on and how to correct it.		
	}
}

- (void)chartHandling {
	Chart *chart = [[Chart alloc] init];
	NSMutableData *imageData = [[NSMutableData alloc] init];
	NSString *imageSizeString;
	
	BOOL imageProcessing = NO;
	
	while ([self.blockBuffer count] > 0) {	
		NSData *data = [self.blockBuffer deQueue];
		NSString *string = [self dataToString:data];
		
		if ([string rangeOfString:@"<ImageBegin>"].location == 0) {
			imageProcessing = YES;
		}
		
		// Get the lines that are strings
		if (imageProcessing) {
			if ([string rangeOfString:@"<ImageBegin>"].location != NSNotFound) {
				NSRange imageBegin = [string rangeOfString:@"<ImageBegin>"];
				NSUInteger length = imageBegin.length;
				imageBegin.location = length;
				imageBegin.length = [data length] - length;
				data = [data subdataWithRange:imageBegin];
				string = [self dataToString:data];
			} 
				
			if ([string rangeOfString:@"<ImageEnd>"].location != NSNotFound) {
				NSRange imageEnd = [string rangeOfString:@"<ImageEnd>"];
				imageEnd.length = imageEnd.location;
				imageEnd.location = 0;
				data = [data subdataWithRange:imageEnd];
				imageProcessing = NO;
			}
			[imageData appendData:data];
		} else {
			NSArray *partsOfString = [string componentsSeparatedByString:@":"];
			if ([partsOfString count] > 1) {
				NSString *dataPortion = [[self stripOffFirstElement:partsOfString] objectAtIndex:0];
				NSString *cleanedDataPortion = [self cleanString:dataPortion];
				if ([string rangeOfString:@"SecOid:"].location == 0) {
					chart.feedTicker = cleanedDataPortion;
				} else if ([string rangeOfString:@"Width:"].location == 0) {
					chart.width = [cleanedDataPortion integerValue];
				} else if ([string rangeOfString:@"Height:"].location == 0) {
					chart.height = [cleanedDataPortion integerValue];
				} else if ([string rangeOfString:@"ImgType:"].location == 0) {
					chart.imageType = cleanedDataPortion;
				} else if ([string rangeOfString:@"ImageSize:"].location == 0) {
					imageSizeString = cleanedDataPortion;
					chart.size = [cleanedDataPortion integerValue];
				}
			}
		}
	}
	if ([imageData length] != chart.size) {
		NSException *exception = [NSException exceptionWithName:@"ImageSizeMismatch" 
														 reason:@"ImageSize specified didn't match received data size." 
													   userInfo:[NSDictionary dictionaryWithObjectsAndKeys:imageSizeString, @"ImageSize", imageData, @"ImageData", nil]];
		[exception raise];
	}
	
	chart.image = imageData;
	[imageData release];

	if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(chart:)]) {
		[self.mTraderServerDataDelegate chart:chart];
	}
	[chart release];
	state = PROCESSING;		
}

- (void)addSecurityOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"SecInfo:"].location == 0) {		
		[stockAddDelegate addOK];
		NSArray *quotes = [self quotesParsing:string];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
			[self.mTraderServerDataDelegate updateQuotes:quotes];
		}
		state = PROCESSING;
	}
}

- (void)removeSecurityOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"SecOid:"].location == 0) {
		NSString *feedTicker = [self cleanString:[[string componentsSeparatedByString:@":"] objectAtIndex:1]];
		if (self.mTraderServerDataDelegate && [self.mTraderServerDataDelegate respondsToSelector:@selector(removedSecurity:)]) {
			[self.mTraderServerDataDelegate removedSecurity:feedTicker];
		}
	}
	
	state = PROCESSING;
}

- (void)staticDataOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"SecOid:"].location == 0) {
		[self staticDataParsing:string];
	}
}

#pragma mark Parsing
/**
 * These are methods that parse blocks of data
 *
 */
- (void)staticDataParsing:(NSString *)secOid {
	NSMutableDictionary *dataDictionary = [[NSMutableDictionary alloc] init];

	if ([secOid rangeOfString:@"SecOid:"].location == 0) {
		NSString *feedTicker = [self cleanString:[[secOid componentsSeparatedByString:@":"] objectAtIndex:1]];
		[dataDictionary setObject:feedTicker forKey:@"feedTicker"];
	}
	
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
		
	if ([string rangeOfString:@"Staticdata:"].location == 0) {
		NSRange staticDataRange = [string rangeOfString:@"Staticdata: "];
		NSRange restOfTheDataRange;
		restOfTheDataRange.location = staticDataRange.length;
		restOfTheDataRange.length = [string length] - staticDataRange.length;
		NSString *staticDataString = [string substringWithRange:restOfTheDataRange];
						
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
	if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(staticUpdates:)]) {
		[self.mTraderServerDataDelegate staticUpdates:dataDictionary];
	}
	state = PROCESSING;
	[dataDictionary release];
}

/**
 * Two possibilities... One quote, or multiple quotes separated by a pipe
 *
 */
- (NSArray *)quotesParsing:(NSString *)quotes {
	NSString *quotesSansCRLF = [self cleanString:quotes];
	NSArray *quotesAndTheRest = [self stripOffFirstElement:[quotesSansCRLF componentsSeparatedByString:@":"]];
	NSString *theRest = [self cleanString:[quotesAndTheRest objectAtIndex:0]];
	return [theRest componentsSeparatedByString:@"|"];
}

- (void)symbolsParsing:(NSString *)symbols {
	// remove the part of the string preceding the colon, and the rest of the symbols are colon separated.
	NSString *symbolsSansCRLF = [self cleanString:symbols];
	NSArray *rows = [self stripOffFirstElement:[symbolsSansCRLF componentsSeparatedByString:@":"]];
	
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

-(NSString *) dataFromRHS:(NSString *)string {
	NSArray *array = [string componentsSeparatedByString:@":"];
	NSString *value;
	
	if ([array count] > 1) {
		array = [self stripOffFirstElement:array];
		value = [array componentsJoinedByString:@":"];
	} else {
		value = nil;
	}
	return value;
}

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
	NSString *final = [NSString stringWithString:string];
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
