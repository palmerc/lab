//
//  mTraderCommunicator.m
//  mTraderCommunicator is a Singleton that the rest of the application can use to
//  communicate with the mTraderServer
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

#import "QFields.h"
#import "mTraderServerMonitor.h"
#import "NSMutableArray+QueueAdditions.h"
#import "UserDefaults.h"
#import "StringHelpers.h"

@implementation mTraderCommunicator

static mTraderCommunicator *sharedCommunicator = nil;
@synthesize symbolsDelegate;
@synthesize mTraderServerMonitorDelegate;
@synthesize communicator = _communicator;
@synthesize defaults = _defaults;
@synthesize blockBuffer = _blockBuffer;
@synthesize qFields = _qFields;
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
		_communicator.dataDelegate = self;
		_defaults = [UserDefaults sharedManager];
		
		_blockBuffer = [[NSMutableArray alloc] init];
		_qFields = nil;
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
- (void)dataReceived:(NSArray *)block {
	[self.blockBuffer addObjectsFromArray:block];
	[self stateMachine];
}

-(void) stateMachine {
	while ([self.blockBuffer count] > 0) {
		//NSLog(@"STATE: %d", state);
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
			case NEWSLIST:
				[self newsListOK];
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
			case HISTDATA:
				[self historyDataOK];
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
		NSString *rhs = [StringHelpers cleanString:[self dataFromRHS:string]];
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
		if (self.mTraderServerMonitorDelegate && [self.mTraderServerMonitorDelegate respondsToSelector:@selector(loginFailed:)]) {
			[self.mTraderServerMonitorDelegate loginFailed:@"UsrPwd"];
		}
		state = LOGIN;
	} else if ([string rangeOfString:@"Request: login/failed.DeniedAccess"].location == 0) {
		if (self.mTraderServerMonitorDelegate && [self.mTraderServerMonitorDelegate respondsToSelector:@selector(loginFailed:)]) {
			[self.mTraderServerMonitorDelegate loginFailed:@"DeniedAccess"];
		}
		state = LOGIN;
	} 
}

-(void) staticResponse {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"Request: login/OK"].location == 0) {
		if (self.mTraderServerMonitorDelegate && [self.mTraderServerMonitorDelegate respondsToSelector:@selector(loginSuccess)]) {
			[self.mTraderServerMonitorDelegate loginSuccessful];
		}		
		state = PREPROCESSING;
	} else if  ([string rangeOfString:@"Request: addSec/OK"].location == 0) {
		state = ADDSEC;
	} else if  ([string rangeOfString:@"Request: remSec/OK"].location == 0) {
		state = REMSEC;
	} else if ([string rangeOfString:@"Request: Chart/OK"].location == 0) {
		state = CHART;
	} else if ([string rangeOfString:@"Request: StaticData/OK"].location == 0) {
		state = STATDATA;
	} else if ([string rangeOfString:@"Request: HistTrades/OK"].location == 0) {
		state = HISTDATA;
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
	} else if ([string rangeOfString:@"Request: NewsList/OK"].location == 0) {
		state = NEWSLIST;
	} else if ([string rangeOfString:@"Request: Logout/OK"].location == 0) {
		state = LOGIN;
	}
}

- (void)loginHandling {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"Request: login/OK"].location == 0) {
		if (self.mTraderServerMonitorDelegate && [self.mTraderServerMonitorDelegate respondsToSelector:@selector(loginSuccessful)]) {
			[self.mTraderServerMonitorDelegate loginSuccessful];
		}

		state = PREPROCESSING;
	} else if ([string rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		if (self.mTraderServerMonitorDelegate && [self.mTraderServerMonitorDelegate respondsToSelector:@selector(loginFailed:)]) {
			[self.mTraderServerMonitorDelegate loginFailed:@"UsrPwd"];
		}
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
		NSArray *newsFeeds = [self exchangesParsing:string];
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(processNewsFeeds:)]) {
			[self.symbolsDelegate processNewsFeeds:newsFeeds];
		}
		state = QUOTE;
	} else if ([string rangeOfString:@"Securities:"].location == 0) {
		NSString *symbolsSansCRLF = [StringHelpers cleanString:string];
		NSArray *rows = [self stripOffFirstElement:[symbolsSansCRLF componentsSeparatedByString:@":"]];
		if (([rows count] == 1) && ([[rows objectAtIndex:0] isEqualToString:@""])) {
			symbolsDefined = NO;
		} else {
			symbolsDefined = YES;
			NSString *symbols = [rows componentsJoinedByString:@":"];
			if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(processSymbols:)]) {
				[self.symbolsDelegate processSymbols:symbols];
			}
		}
	} else if ([string rangeOfString:@"Exchanges:"].location == 0) {
		NSArray *symbolFeeds = [self exchangesParsing:string];
		if (symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(processSymbolFeeds:)]) {
			[self.symbolsDelegate processSymbolFeeds:symbolFeeds];
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
				NSString *cleanedDataPortion = [StringHelpers cleanString:dataPortion];
				if ([string rangeOfString:@"SecOid:"].location == 0) {
					[chart  setObject:cleanedDataPortion forKey:@"feedTicker"];
				} else if ([string rangeOfString:@"Width:"].location == 0) {
					[chart setObject:[NSNumber numberWithInteger:[cleanedDataPortion integerValue]] forKey:@"width"];
				} else if ([string rangeOfString:@"Height:"].location == 0) {
					[chart setObject:[NSNumber numberWithInteger:[cleanedDataPortion integerValue]] forKey:@"height"];
				} else if ([string rangeOfString:@"ImgType:"].location == 0) {
					[chart setObject:cleanedDataPortion forKey:@"type"];
				} else if ([string rangeOfString:@"ImageSize:"].location == 0) {
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
		NSString *symbolsSansCRLF = [StringHelpers cleanString:string];
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
		NSString *feedTicker = [StringHelpers cleanString:[[string componentsSeparatedByString:@":"] objectAtIndex:1]];
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(removedSymbol:)]) {
			[self.symbolsDelegate removedSymbol:feedTicker];
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

- (void)historyDataOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"SecOid:"].location == 0) {
		[self historyDataParsing:string];
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
		NSString *feedTicker = [StringHelpers cleanString:[[secOid componentsSeparatedByString:@":"] objectAtIndex:1]];
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

- (void)historyDataParsing:(NSString *)secOid {
	NSMutableDictionary *dataDictionary = [[NSMutableDictionary alloc] init];
	
	if ([secOid rangeOfString:@"SecOid:"].location == 0) {
		NSString *feedTicker = [StringHelpers cleanString:[[secOid componentsSeparatedByString:@":"] objectAtIndex:1]];
		[dataDictionary setObject:feedTicker forKey:@"feedTicker"];
	}
	
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"FirstTrade:"].location == 0) {
		NSString *firstTrade = [StringHelpers cleanString:[[string componentsSeparatedByString:@":"] objectAtIndex:1]];
		[dataDictionary setObject:firstTrade forKey:@"firstTrade"];
	}
	
	data = [self.blockBuffer deQueue];
	string = [self dataToString:data];
	
	if ([string rangeOfString:@"CountTrades:"].location == 0) {
		NSString *firstTrade = [StringHelpers cleanString:[[string componentsSeparatedByString:@":"] objectAtIndex:1]];
		[dataDictionary setObject:firstTrade forKey:@"countTrades"];
	}
	
	data = [self.blockBuffer deQueue];
	string = [self dataToString:data];
	
	if ([string rangeOfString:@"Trades:"].location == 0) {
		NSArray *tradesArray = [string componentsSeparatedByString:@":"];
		tradesArray = [self stripOffFirstElement:tradesArray];
		
		NSString *tradesString = [tradesArray componentsJoinedByString:@":"];
		tradesString = [StringHelpers cleanString:tradesString];
		
		[dataDictionary setObject:tradesString forKey:@"trades"];
	}
	
	if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(tradesUpdate:)]) {
		[self.symbolsDelegate tradesUpdate:dataDictionary];
	}
	state = PROCESSING;
	[dataDictionary release];
}

- (void)newsListFeedsOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [self dataToString:data];
	
	if ([string rangeOfString:@"News:"].location == 0) {
		NSString *newsArticles = [self dataToString:data];
		NSArray *colonSeparatedComponents = [newsArticles componentsSeparatedByString:@":"];
		colonSeparatedComponents = [self stripOffFirstElement:colonSeparatedComponents];
		newsArticles = [colonSeparatedComponents componentsJoinedByString:@":"];
		NSArray *newsArticlesArray = [newsArticles componentsSeparatedByString:@"|"];
		// 1073/01226580;;22.01;14:36;DJ Vattenfall To Sell Nuon Deutschland To Municipal Utility Group
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(newsListFeedsUpdates:)]) {
			[self.symbolsDelegate newsListFeedsUpdates:newsArticlesArray];
		}
		state = PROCESSING;
	}
}

- (void)newsListOK {	
	NSString *feedTicker = nil;
	while ([self.blockBuffer count] > 0) {
		NSData *data = [self.blockBuffer deQueue];
		NSString *string = [self dataToString:data];

		if ([string rangeOfString:@"SecOid:"].location == 0) {
			NSArray *colonSeparatedComponents = [string componentsSeparatedByString:@":"];
			NSArray *cleanedComponents = [StringHelpers cleanComponents:colonSeparatedComponents];
			
			NSRange feedTickerRange;
			feedTickerRange.location = 1;
			feedTickerRange.length = [cleanedComponents count] - 1;
			NSArray *feedTickerComponents = [cleanedComponents subarrayWithRange:feedTickerRange];
			feedTicker = [feedTickerComponents componentsJoinedByString:@":"];
		} else if ([string rangeOfString:@"News:"].location == 0) {
			NSString *newsArticles = [self dataToString:data];
			NSArray *colonSeparatedComponents = [newsArticles componentsSeparatedByString:@":"];
			NSRange newsArticleRange;
			newsArticleRange.location = 1;
			newsArticleRange.length = [colonSeparatedComponents count] - 1;
			colonSeparatedComponents = [colonSeparatedComponents subarrayWithRange:newsArticleRange];			
			newsArticles = [colonSeparatedComponents componentsJoinedByString:@":"];
			
			NSArray *newsArticlesArray = [StringHelpers cleanComponents:[newsArticles componentsSeparatedByString:@"|"]];
			NSMutableArray *feedTickerNewsArticlesMutableArray = [[NSMutableArray alloc] initWithArray:newsArticlesArray];
			[feedTickerNewsArticlesMutableArray insertObject:feedTicker atIndex:0];
			NSArray *feedTickerNewsArticlesArray = [NSArray arrayWithArray:feedTickerNewsArticlesMutableArray];
			[feedTickerNewsArticlesMutableArray release];
			
			// 1073/01226580;;22.01;14:36;DJ Vattenfall To Sell Nuon Deutschland To Municipal Utility Group
			if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(newsListFeedsUpdates:)]) {
				[self.symbolsDelegate symbolNewsUpdates:feedTickerNewsArticlesArray];
			}
			state = PROCESSING;
		}
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
- (NSArray *)quotesParsing:(NSString *)quotesString {
	if (self.qFields == nil) {
		return nil;
	}
	
	NSArray *quotesAndTheRest = [self stripOffFirstElement:[quotesString componentsSeparatedByString:@":"]];
	quotesAndTheRest = [StringHelpers cleanComponents:quotesAndTheRest];
	NSString *theRest = [quotesAndTheRest componentsJoinedByString:@":"];
	
	NSArray *quotesStrings = [theRest componentsSeparatedByString:@"|"];
	quotesStrings = [StringHelpers cleanComponents:quotesStrings];
	
	NSMutableArray *quotes = [[[NSMutableArray alloc] init] autorelease];
	for (NSString *quoteString in quotesStrings) {
		NSArray *values = [quoteString componentsSeparatedByString:@";"];
		NSDictionary *aFormattedQuote = [self.qFields dictionaryFromQuote:values];
		[quotes addObject:aFormattedQuote];		
	}
	
	return quotes;
}

- (NSArray *)exchangesParsing:(NSString *)exchanges {
	NSArray *arrayOfComponents = [self stripOffFirstElement:[exchanges componentsSeparatedByString:@":"]];
	exchanges = [arrayOfComponents componentsJoinedByString:@":"];
	NSArray *exchangesArray = [exchanges componentsSeparatedByString:@","];
	exchangesArray = [StringHelpers cleanComponents:exchangesArray];
	return exchangesArray;
}

#pragma mark -
#pragma mark mTrader Server Requests
/**
 * These are methods that format mTrader Server Messages
 *
 */

- (void)login {
	if (! [[mTraderServerMonitor sharedManager] connected] ) {
		return;
	}
	
	NSString *username = self.defaults.username;
	NSString *password = self.defaults.password;
	
	username = [StringHelpers cleanString:username];
	password = [StringHelpers cleanString:password];
	if (username != nil && password != nil && ![username isEqualToString:@""] && ![password isEqualToString:@""]) {
		NSString *version = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
		
		NSString *ActionLogin = @"Action: login";
		NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@/%@", username, password];
		NSString *Platform = [NSString stringWithFormat:@"Platform: %@ %@", [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion]];
		NSString *Client = @"Client: mTrader";
		NSString *Protocol = @"Protocol: 2.0";
		NSString *Version = [NSString stringWithFormat:@"VerType: %@", version];
		NSString *ConnectionType = @"ConnType: Socket";
		NSString *Streaming = @"Streaming: 1";
		
		QFields *qFields = [[QFields alloc] init];
		qFields.timeStamp = YES;
		qFields.lastTrade = YES;
		qFields.bidPrice = YES;
		qFields.askPrice = YES;
		qFields.change = YES;
		qFields.changePercent = YES;
		qFields.changeArrow = YES;
		self.qFields = qFields;
		[qFields release];
		NSString *QFieldsServerString = [NSString stringWithFormat:@"QFields: %@", [qFields getCurrentQFieldsServerString]];
		
		NSArray *loginArray = [NSArray arrayWithObjects:ActionLogin, Authorization, Platform, Client, Version, Protocol, ConnectionType, Streaming, QFieldsServerString, nil];
		NSString *loginString = [self arrayToFormattedString:loginArray];
		
		[self.communicator writeString:loginString];
	}
}

- (void)logout {
	NSString *ActionLogout = @"Action: Logout";
	NSArray *logoutArray = [NSArray arrayWithObjects:ActionLogout, nil];
	NSString *logoutString = [self arrayToFormattedString:logoutArray];
	[self.communicator writeString:logoutString];
	
	state = LOGIN;
}

- (void)addSecurity:(NSString *)tickerSymbol withMCode:(NSString *)mCode {
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] ) {
		return;
	}
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
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] ) {
		return;
	}
	NSString *username = self.defaults.username;
	
	NSString *ActionRemSec = @"Action: remSec";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	
	NSArray *removeSecurityArray = [NSArray arrayWithObjects:ActionRemSec, Authorization, SecOid, nil];
	NSString *removeSecurityString = [self arrayToFormattedString:removeSecurityArray];
	
	[self.communicator writeString:removeSecurityString];
}

- (void)staticDataForFeedTicker:(NSString *)feedTicker {
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] ) {
		return;
	}
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

/**
 *
 * If feedTicker is nil then this applies to all symbols.
 *
 */
- (void)setStreamingForFeedTicker:(NSString *)feedTicker {
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] || (self.qFields == nil) ) {
		return;
	}
	
	NSMutableArray *serverRequestArray = [[NSMutableArray alloc] init];
	
	// Action: q
	NSString *action = @"Action: q";
	[serverRequestArray addObject:action];
	
	// Authorization: user
	NSString *username = self.defaults.username;
	NSString *authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	[serverRequestArray addObject:authorization];
	
	// SecOid:
	// stream all symbols
	if (feedTicker != nil) {
		NSString *secOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
		[serverRequestArray addObject:secOid];
	}
	
	
	// QFields:
	NSString *QFieldsString = [NSString stringWithFormat:@"QFields: %@", [self.qFields getCurrentQFieldsServerString]];
	[serverRequestArray addObject:QFieldsString];
	
	NSString *serverFormattedRequest = [self arrayToFormattedString:serverRequestArray];
	[self.communicator writeString:serverFormattedRequest];
	
	[serverRequestArray release];
}

- (void)tradesRequest:(NSString *)feedTicker {
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] ) {
		return;
	}
	NSInteger index = -1;
	NSInteger count = 30;
	
	NSString *username = self.defaults.username;
	NSString *ActionStatData = @"Action: HistTrades";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	NSString *Index = [NSString stringWithFormat:@"Index: %d", index];
	NSString *Count = [NSString stringWithFormat:@"Count: %d", count];
	NSString *Columns = [NSString stringWithFormat:@"Columns: %@", @"TPVAY"];
	
	NSArray *getTradesArray = [NSArray arrayWithObjects:ActionStatData, Authorization, SecOid, Index, Count, Columns, nil];
	NSString *tradesRequestString = [self arrayToFormattedString:getTradesArray];
	
	[self.communicator writeString:tradesRequestString];
}

- (void)graphForFeedTicker:(NSString *)feedTicker period:(NSUInteger)period width:(NSUInteger)width height:(NSUInteger)height orientation:(NSString *)orientation {
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] ) {
		return;
	}
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

// News Requests
- (void)newsItemRequest:(NSString *)newsId {
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] ) {
		return;
	}
	NSString *username = self.defaults.username;
	NSString *ActionNewsBody = @"Action: NewsBody";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	//NSString *Formatting = [NSString stringWithString:@"Reformat: 0"];
	NSString *NewsID = [NSString stringWithFormat:@"NewsID: %@", newsId];
	
	NSArray *newsItemArray = [NSArray arrayWithObjects:ActionNewsBody, Authorization, NewsID, nil];
	NSString *newsItemRequestString = [self arrayToFormattedString:newsItemArray];
	
	[self.communicator writeString:newsItemRequestString];
}

- (void)newsListFeed:(NSString *)mCode {
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] ) {
		return;
	}
	NSString *username = self.defaults.username;
	NSString *ActionNewsListFeeds = @"Action: NewsListFeeds";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *newsFeeds = [NSString stringWithFormat:@"NewsFeeds: [%@]", mCode];
	NSString *days = @"Days: 30";
	NSString *maxCount = @"MaxCount: 50";
	
	NSArray *getNewsListFeedsArray = [NSArray arrayWithObjects:ActionNewsListFeeds, Authorization, newsFeeds, days, maxCount, nil];
	NSString *newsListFeedsString = [self arrayToFormattedString:getNewsListFeedsArray];
	
	[self.communicator writeString:newsListFeedsString];
}

- (void)symbolNewsForFeedTicker:(NSString *)feedTicker {
	if ( ![[mTraderServerMonitor sharedManager] loggedIn] ) {
		return;
	}
	NSString *username = self.defaults.username;
	NSString *ActionNewsList = @"Action: NewsList";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	NSString *days = @"Days: 30";
	NSString *maxCount = @"MaxCount: 50";
	
	NSArray *getSymbolNewsListFeedsArray = [NSArray arrayWithObjects:ActionNewsList, Authorization, SecOid, days, maxCount, nil];
	NSString *symbolNewsListFeedsString = [self arrayToFormattedString:getSymbolNewsListFeedsArray];
	
	[self.communicator writeString:symbolNewsListFeedsString];	
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

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_qFields release];
	[_blockBuffer release];
	[_communicator release];
	
	[super dealloc];
}

@end
