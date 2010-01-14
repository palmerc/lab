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
@synthesize communicator = _communicator;
@synthesize defaults = _defaults;
@synthesize isLoggedIn = _isLoggedIn;
@synthesize blockBuffer = _blockBuffer;
@synthesize currentLine = _currentLine;

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

- (id)init {
	self = [super init];
	if (self != nil) {
		_communicator = [[Communicator alloc] initWithSocket:@"wireless.theonlinetrader.com" port:7780];
		_communicator.delegate = self;
		_defaults = [UserDefaults sharedManager];
		_isLoggedIn = NO;
		_loginStatusHasChanged = NO;
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

// Delegate method from Communicator
- (void)dataReceived {
	//UIApplication* app = [UIApplication sharedApplication]; 
	//app.networkActivityIndicatorVisible = YES; 
	self.currentLine = [self.communicator readLine];
	
	//NSLog(@"CurrentLine: >>%@<<", [self currentLineToString]);
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
		case STREAMING:
			[self streamingLoop];
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

- (void)chartHandling {
	[self.blockBuffer addObject:self.currentLine];
	//SecOid: feedTicker
	//Width: x
	//Height: x
	//ImgType: PNG // or GIF
	//ImageSize: xxx
	//<ImageBegin>...<ImageEnd>
	NSString *line = [self currentLineToString];
	if ([line rangeOfString:@"<ImageEnd>"].location != NSNotFound) {
		Chart *chart = [self chartParsing];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(chart:)]) {
			[self.mTraderServerDataDelegate chart:chart];
		}
		[self blockBufferRenew];
		state = PROCESSING;
	}
}

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
		_loginStatusHasChanged = YES;
		_isLoggedIn = YES;
		state = PREPROCESSING;
	} else if ([line rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		_loginStatusHasChanged = YES;
		_isLoggedIn = NO;
	}
}

- (void)preprocessing {
	NSString *line = [self currentLineToString];
	[self.blockBuffer addObject:line];
	if ([line rangeOfString:@"Quotes:"].location == 0) {
		[self settingsParsing];		
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(serverSettings:)]) {
			[self.mTraderServerDataDelegate serverSettings:self.blockBuffer];
		}
		[self blockBufferRenew];
		state = PROCESSING;
	}
}

- (void)processingLoop {
	NSString *line = [self currentLineToString];
	[self.blockBuffer addObject:line];
	if ([line rangeOfString:@"HTTP/1.1 200 OK"].location == 0) { // Static data
		state = STATIC;
	} else if ([line rangeOfString:@"Request: q"].location == 0) { // Streaming
		state = QUOTE;
	} else if ([line isEqualToString:@""]) {
		// Ignore blank lines
	} else {
		NSLog(@"Undefined state: %@", self.currentLine);
	}
	
}

- (void)staticLoop {
	// Request: q
	// Content-Length:
	// Request: Chart/OK
	// Request: addSec/OK
	// Request: remSec/OK
	// Request: addSec/failed.NoSuchSec
	// Request: addSec/failed.AlreadyExists
	// Request: remSec/CouldNotDelete
	
	NSString *line = [self currentLineToString];
	[self.blockBuffer addObject:line];
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
		[self blockBufferRenew];
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
	NSString *line = [self currentLineToString];
	[self.blockBuffer addObject:line];
	if ([line rangeOfString:@"\r\n"].location == 0) {
		NSLog(@"Keep-Alive");
		state = PROCESSING;
	} else if ([line rangeOfString:@"Quotes: "].location == 0) {
		NSArray *quotes = [self quotesParsing:line];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
			[self.mTraderServerDataDelegate updateQuotes:quotes];
		}
		[self blockBufferRenew];
		state = PROCESSING;
		// call the delegate
	} else if ([line rangeOfString:@"Kickout: 1"].location == 0) {
		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Kickout" message:@"You have been logged off since you logged in from another client" delegate:self cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
		[alertView show];
		[alertView release];
		state = KICKOUT;
		// Tell the user they are no longer logged on and how to correct it.		
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
		[self blockBufferRenew];
		state = PROCESSING;
	}
}

- (void)staticDataOK {
	NSString *line = [self currentLineToString];
	[self.blockBuffer addObject:line];
	if ([line rangeOfString:@"SecOid:"].location == 0) {
		// Just save it
	} else if ([line rangeOfString:@"Staticdata: "].location == 0) {

		[self staticDataParsing];
		[self blockBufferRenew];
		state = PROCESSING;
	}
}

- (void)blockBufferRenew {
	NSLog(@"Block: %@", self.blockBuffer);
	[self.blockBuffer release];
	_blockBuffer = [[NSMutableArray alloc] init];	
}

- (void)settingsParsing {
	for (NSString *line in self.blockBuffer) {
		if ([line rangeOfString:@"Symbols:"].location == 0) {
			[self symbolsParsing:line];
		} else if ([line rangeOfString:@"Quotes:"].location == 0) {
			NSArray *quotes = [self quotesParsing:line];
			if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
				[self.mTraderServerDataDelegate updateQuotes:quotes];
			}
			[self blockBufferRenew];
			state = PROCESSING;
			break;
		}
	}
}

- (void)staticDataParsing {
	for (NSString *line in self.blockBuffer) {
		if ([line rangeOfString:@"Staticdata:"].location == 0) {
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
					NSLog(@"%@ <=> %@", key, value);
				}
				
			}
		}
	}
}

- (Chart *)chartParsing {
	Chart *chart = [[Chart alloc] init];
	for (NSData *data in self.blockBuffer) {
		NSString *line = [self dataToString:data];
		if ([line rangeOfString:@"<ImageBegin>"].location == NSNotFound) {
			NSArray *partsOfString = [line componentsSeparatedByString:@":"];
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
				chart.size = [cleanedDataPortion integerValue];
			}
		} else {
			break;
		}
	}
	
	NSRange range;
	range.location = 0;
	range.length = 5;
	NSIndexSet *indexes = [NSIndexSet indexSetWithIndexesInRange:range];
	[self.blockBuffer removeObjectsAtIndexes:indexes];
	
	NSMutableData *imageData = [[NSMutableData alloc] init];
	for (NSData *data in self.blockBuffer) {
		NSString *line = [self dataToString:data];
		if ([line rangeOfString:@"<ImageBegin>"].location != NSNotFound) {
			NSRange imageBegin = [line rangeOfString:@"<ImageBegin>"];
			NSUInteger length = imageBegin.length;
			imageBegin.location = length;
			imageBegin.length = [data length] - length;
			data = [data subdataWithRange:imageBegin];
		} else if ([line rangeOfString:@"<ImageEnd>"].location != NSNotFound) {
			NSRange imageEnd = [line rangeOfString:@"<ImageEnd>"];
			imageEnd.location = 0;
			imageEnd.length = [data length] - imageEnd.length;
			data = [data subdataWithRange:imageEnd];
		}
		[imageData appendData:data];
	}
	UIImage *image = [[UIImage alloc] initWithData:imageData];
	
	chart.image = image;
	[image release];
	[imageData release];
	return chart;
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

- (BOOL)loginStatusHasChanged {
	BOOL result = NO;
	if (_loginStatusHasChanged) {
		result = YES;
		
		_loginStatusHasChanged = NO;
	}
	return result;
}

- (void)logout {
	NSString *ActionLogout = @"Action: Logout";
	NSArray *logoutArray = [NSArray arrayWithObjects:ActionLogout, nil];
	NSString *logoutString = [self arrayToFormattedString:logoutArray];
	[self.communicator writeString:logoutString];
}

- (void)login {
	NSString *username = self.defaults.username;
	NSString *password = self.defaults.password;
	
	NSString *version = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
	NSString *build = [NSString stringWithFormat:@"%@",[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"]];
	
	NSString *ActionLogin = @"Action: login";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@/%@", username, password];
	NSString *Platform = [NSString stringWithFormat:@"Platform: %@ %@", [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion]];
	NSString *Client = @"Client: iTrader";
	NSString *Version = [NSString stringWithFormat:@"VerType: %@.%@", version, build];
	NSString *ConnectionType = @"ConnType: Socket";
	NSString *Streaming = @"Streaming: 1";
	NSString *QFields = @"QFields: l;cp;b;a;av;bv;c;h;lo;o;v";
	//NSString *QFields = @"QFields: l";
	
	NSArray *loginArray = [NSArray arrayWithObjects:ActionLogin, Authorization, Platform, Client, Version, ConnectionType, Streaming, QFields, nil];
	NSString *loginString = [self arrayToFormattedString:loginArray];
	
	if ([self.communicator isConnected]) {
		[self.communicator stopConnection];
		_isLoggedIn = NO;
	}
	
	[self.communicator startConnection];
	[self.communicator writeString:loginString];
}

- (void)staticDataForFeedTicker:(NSString *)feedTicker {
	NSString *username = self.defaults.username;
	NSString *language = @"EN";
	NSString *ActionStatData = @"Action: StatData";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	NSString *Language = [NSString stringWithFormat:@"Language: %@", language];
	
	NSArray *getStatDataArray = [NSArray arrayWithObjects:ActionStatData, Authorization, SecOid, Language, nil];
	NSString *getStatDataString = [self arrayToFormattedString:getStatDataArray];
	
	[self.communicator writeString:getStatDataString];
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
