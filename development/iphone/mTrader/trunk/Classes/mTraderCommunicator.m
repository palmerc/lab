//
//  mTraderCommunicator.m
//  mTraderCommunicator is a Singleton that the rest of the application can use to
//  communicate with the mTraderServer
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
#import "mTraderServerMonitor.h"
#import "NSMutableArray+QueueAdditions.h"
#import "UserDefaults.h";

@implementation mTraderCommunicator

static mTraderCommunicator *sharedCommunicator = nil;
@synthesize server = _server;
@synthesize port = _port;
@synthesize symbolsDelegate;
@synthesize mTraderServerMonitorDelegate;
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
	NSString *server = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerAddress"]];
	NSString *port = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerPort"]];
	return [self initWithURL:server onPort:[port integerValue]];
}

- (id)initWithURL:(NSString *)url onPort:(NSInteger)port {
	self = [super init];
	if (self != nil) {
		_communicator = [[Communicator alloc] initWithSocket:url onPort:port];
		_communicator.delegate = self;
		_defaults = [UserDefaults sharedManager];
		
		isLoggedIn = NO;
		loginStatusHasChanged = NO;
		_blockBuffer = [[NSMutableArray alloc] init];
		contentLength = 0;
		state = LOGIN;
	}
	return self;
}

#pragma mark Singleton Methods
/**
 * Methods for Singleton implementation
 *
 */
+ (mTraderCommunicator *)sharedManager {
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
	NSLog(@"Received: %@", string);
	if (![string isEqualToString:@"\r\r"]) {
		[self.blockBuffer addObject:data];
	} else {
		[self stateMachine];
		[self.blockBuffer removeAllObjects];
	}
}	
	
-(void) stateMachine {
	while ([self.blockBuffer count] > 0) {
		NSLog(@"STATE: %d", state);
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
			case NEWSFEEDS:
				[self newsListFeedsOK];
				break;
			case NEWSITEM:
				[self newsBodyOK];
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
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(failedToAddNoSuchSecurity)]) {
			[self.symbolsDelegate failedToAddNoSuchSecurity];
		}
		state = PROCESSING;
	} else if 	 ([string rangeOfString:@"Request: addSec/failed.AlreadyExists"].location == 0) {
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(failedToAddAlreadyExists)]) {
			[self.symbolsDelegate failedToAddAlreadyExists];
		}
		state = PROCESSING;
	} else if ([string rangeOfString:@"Request: q"].location == 0) {
		if ([self.blockBuffer count] == 0) {
			state = PROCESSING;
		} else {
			state = QUOTE;
		}
	} else if ([string rangeOfString:@"Request: NewsBody/OK"].location == 0) {
		state = NEWSITEM;
	} else if ([string rangeOfString:@"Request: NewsListFeeds/OK"].location == 0) {
		state = NEWSFEEDS;
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
 * Read the login stream up to quotes, call settingsParsing to deal with it and
 * change state.
 */
- (void)preprocessing {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];

	if ([string rangeOfString:@"NewsFeeds:"].location == 0) {
		//[self newsFeedsParsing];
		if (symbolsDefined == NO) {
			state = PROCESSING;
		}
	} else if ([string rangeOfString:@"Symbols:"].location == 0) {
		NSString *symbolsSansCRLF = [self cleanString:string];
		NSArray *rows = [self stripOffFirstElement:[symbolsSansCRLF componentsSeparatedByString:@":"]];
		if (([rows count] == 1) && ([[rows objectAtIndex:0] isEqualToString:@""])) {
			symbolsDefined = NO;
		} else {
			symbolsDefined = YES;
			NSString *symbols = [rows componentsJoinedByString:@":"];
			if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(replaceAllSymbols:)]) {
				[self.symbolsDelegate replaceAllSymbols:symbols];
			}
			//[self symbolsParsing:string];
		}
	} else if ([string rangeOfString:@"Quotes:"].location == 0) {
		NSArray *quotes = [self quotesParsing:string];
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(updateSymbols:)]) {
			[self.symbolsDelegate updateSymbols:quotes];
		}
		state = PROCESSING;
	} else if ([string rangeOfString:@"Exchanges:"].location == 0) {
		NSArray *exchanges = [self exchangesParsing:string];
		if (symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(addExchanges:)]) {
			[self.symbolsDelegate addExchanges:exchanges];
		}
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
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(updateSymbols:)]) {
			[self.symbolsDelegate updateSymbols:quotes];
		}
		state = PROCESSING;
	} else if ([string rangeOfString:@"Kickout: 1"].location == 0) {
		state = KICKOUT;
		if (self.mTraderServerMonitorDelegate && [self.mTraderServerMonitorDelegate respondsToSelector:@selector(kickedOut)]) {
			[self.mTraderServerMonitorDelegate kickedOut];
		}
	} else {
		state = PROCESSING;
	}
}

- (void)chartHandling {
	NSMutableDictionary *chart = [[NSMutableDictionary alloc] init];
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
					[chart  setObject:cleanedDataPortion forKey:@"feedTicker"];
				} else if ([string rangeOfString:@"Width:"].location == 0) {
					[chart setObject:[NSNumber numberWithInteger:[cleanedDataPortion integerValue]] forKey:@"width"];
				} else if ([string rangeOfString:@"Height:"].location == 0) {
					[chart setObject:[NSNumber numberWithInteger:[cleanedDataPortion integerValue]] forKey:@"height"];
				} else if ([string rangeOfString:@"ImgType:"].location == 0) {
					[chart setObject:cleanedDataPortion forKey:@"type"];
				} else if ([string rangeOfString:@"ImageSize:"].location == 0) {
					imageSizeString = cleanedDataPortion;
					[chart setObject:[NSNumber numberWithInteger:[cleanedDataPortion integerValue]] forKey:@"size"];
				}
			}
		}
	}
	
	[chart setObject:(NSData *)imageData forKey:@"data"];
	[imageData release];

	if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(chartUpdate:)]) {
		[self.symbolsDelegate chartUpdate:chart];
	}
	[chart release];
	state = PROCESSING;		
}

- (void)addSecurityOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"SecInfo:"].location == 0) {	
		NSString *symbolsSansCRLF = [self cleanString:string];
		NSArray *rows = [self stripOffFirstElement:[symbolsSansCRLF componentsSeparatedByString:@":"]];
		string = [rows objectAtIndex:0];
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(addSymbols:)]) {
			[self.symbolsDelegate addSymbols:string];
		}
		state = PROCESSING;
	}
}

- (void)removeSecurityOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"SecOid:"].location == 0) {
		NSString *feedTicker = [self cleanString:[[string componentsSeparatedByString:@":"] objectAtIndex:1]];
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(removedSecurity:)]) {
			[self.symbolsDelegate removedSecurity:feedTicker];
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
	if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(staticUpdates:)]) {
		[self.symbolsDelegate staticUpdates:dataDictionary];
	}
	state = PROCESSING;
	[dataDictionary release];
}

-(void) newsListFeedsOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"News:"].location == 0) {
		NSString *newsArticles = [self dataToString:data];
		NSArray *newsArticlesArray = [newsArticles componentsSeparatedByString:@"|"];
		// 1073/01226580;;22.01;14:36;DJ Vattenfall To Sell Nuon Deutschland To Municipal Utility Group
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(newsListFeedsUpdates:)]) {
			[self.symbolsDelegate newsListFeedsUpdates:newsArticlesArray];
		}
		state = PROCESSING;
	}
}

- (void)newsBodyOK {
	NSMutableArray *newsItem = [[NSMutableArray alloc] init];
	while ([self.blockBuffer count] > 0) {
		NSData *data = [self.blockBuffer deQueue];
		NSString *string = [self dataToString:data];
		string = [self dataFromRHS:string];
		[newsItem addObject:string];
	}	
	
	if ([self.blockBuffer count] == 0) {
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(newsItemUpdate:)]) {
			[self.symbolsDelegate newsItemUpdate:newsItem];
		}
		
		state = PROCESSING;
	}
	[newsItem release];
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

- (NSArray *)exchangesParsing:(NSString *)exchanges {
	NSArray *arrayOfComponents = [self stripOffFirstElement:[exchanges componentsSeparatedByString:@":"]];
	NSString *dataPortion = [self cleanString:[arrayOfComponents objectAtIndex:0]];
	NSArray *exchangesArray = [dataPortion componentsSeparatedByString:@","];
	return exchangesArray;
}

- (BOOL)loginStatusHasChanged {
	BOOL result = NO;
	if (loginStatusHasChanged) {
		result = YES;
		
		loginStatusHasChanged = NO;
	}
	return result;
}

#pragma mark -
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
		NSString *build = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"]];
		
		NSString *ActionLogin = @"Action: login";
		NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@/%@", username, password];
		NSString *Platform = [NSString stringWithFormat:@"Platform: %@ %@", [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion]];
		NSString *Client = @"Client: mTrader";
		NSString *Version = [NSString stringWithFormat:@"VerType: %@.%@", version, build];
		NSString *ConnectionType = @"ConnType: Socket";
		NSString *Streaming = @"Streaming: 1";
		NSString *QFields = @"QFields: l;cp;b;a;c";
		
		NSArray *loginArray = [NSArray arrayWithObjects:ActionLogin, Authorization, Platform, Client, Version, ConnectionType, Streaming, QFields, nil];
		NSString *loginString = [self arrayToFormattedString:loginArray];
		
		if ([self.communicator isConnected]) {
			[self.communicator stopConnection];
			isLoggedIn = NO;
		}
		
		[self.communicator startConnection];
		[self.communicator writeString:loginString];
	}
}

- (void)orderBookForFeedTicker:(NSString *)feedTicker {
	NSString *username = self.defaults.username;
	NSString *ActionQ = @"Action: q";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	NSString *QFields = @"QFields: l;cp;v;d";
	NSArray *getOrderBookArray = [NSArray arrayWithObjects:ActionQ, Authorization, SecOid, QFields, nil];

	NSString *orderBookRequestString = [self arrayToFormattedString:getOrderBookArray];
	[self.communicator writeString:orderBookRequestString];
}

- (void)stopStreamingData {	
	NSString *username = self.defaults.username;
	NSString *ActionQ = @"Action: q";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *QFields = @"QFields:";
	NSArray *stopStreamingArray = [NSArray arrayWithObjects:ActionQ, Authorization, QFields, nil];
	
	NSString *stopStreamingRequestString = [self arrayToFormattedString:stopStreamingArray];
	[self.communicator writeString:stopStreamingRequestString];
	
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

- (void)newsItemRequest:(NSString *)newsId {
	NSString *username = self.defaults.username;
	NSString *ActionNewsBody = @"Action: NewsBody";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *NewsID = [NSString stringWithFormat:@"NewsID: %@", newsId];
	
	NSArray *newsItemArray = [NSArray arrayWithObjects:ActionNewsBody, Authorization, NewsID, nil];
	NSString *newsItemRequestString = [self arrayToFormattedString:newsItemArray];
	
	[self.communicator writeString:newsItemRequestString];
}

- (void)newsListFeeds {
	NSString *username = self.defaults.username;
	NSString *ActionNewsListFeeds = @"Action: NewsListFeeds";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *newsFeeds = @"NewsFeeds: AllNews";
	NSString *days = @"Days: 30";
	NSString *maxCount = @"MaxCount: 50";
	
	NSArray *getNewsListFeedsArray = [NSArray arrayWithObjects:ActionNewsListFeeds, Authorization, newsFeeds, days, maxCount, nil];
	NSString *newsListFeedsString = [self arrayToFormattedString:getNewsListFeedsArray];
	
	[self.communicator writeString:newsListFeedsString];
}

- (void)chainsStreaming {
	NSString *username = self.defaults.username;
	NSString *ActionQ = @"Action: q";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid:"];
	NSString *QFields = @"QFields: l;cp;b;a;c";
	NSArray *getDynamicDetailArray = [NSArray arrayWithObjects:ActionQ, Authorization, SecOid, QFields, nil];
	
	NSString *dynamicDetailRequestString = [self arrayToFormattedString:getDynamicDetailArray];
	[self.communicator writeString:dynamicDetailRequestString];	
}

- (void)dynamicDetailForFeedTicker:(NSString *)feedTicker {
	NSString *username = self.defaults.username;
	NSString *ActionQ = @"Action: q";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	NSString *QFields = @"QFields: l;h;lo;o;v";
	NSArray *getDynamicDetailArray = [NSArray arrayWithObjects:ActionQ, Authorization, SecOid, QFields, nil];
	
	NSString *dynamicDetailRequestString = [self arrayToFormattedString:getDynamicDetailArray];
	[self.communicator writeString:dynamicDetailRequestString];	
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

- (void)addSecurity:(NSString *)tickerSymbol withMCode:(NSString *)mCode {
	NSString *username = self.defaults.username;
	
	NSString *ActionAddSec = @"Action: addSec";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *Search = [NSString stringWithFormat:@"Search: %@", tickerSymbol];
	NSString *MCode = [NSString stringWithFormat:@"mCode: [%@]", mCode];
	
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

#pragma mark -
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
	return [array subarrayWithRange:rowsWithoutFirstString];
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

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[self.blockBuffer release];
	[self.communicator release];
	[super dealloc];
}

@end
