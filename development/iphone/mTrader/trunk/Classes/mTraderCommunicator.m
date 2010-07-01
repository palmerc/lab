//
//  mTraderCommunicator.m
//  mTraderCommunicator is a Singleton that the rest of the application can use to
//  communicate with the mTraderServer
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

#define DEBUG 0
#define DEBUG_STATE 0

#import "mTraderCommunicator.h"

#import "NSMutableArray+QueueAdditions.h"
#import "NSData+StringAdditions.h"

#import "DataController.h"
#import "QFields.h"
#import "mTraderServerMonitor.h"
#import "UserDefaults.h"
#import "StringHelpers.h"


@interface mTraderCommunicator ()
// State machine methods
- (void)stateMachine;
- (void)headerParsing;
- (void)fixedLength;
- (void)staticResponse;
- (void)chartHandling;
- (void)loginHandling;
- (void)preprocessing;
- (void)processingLoop;
- (void)quoteHandling;
- (void)searchNoHit;
- (void)searchResultsHandling;
- (void)addSecurityOK;
- (void)removeSecurityOK;
- (void)newsListFeedsOK;
- (void)newsListOK;
- (void)newsBodyOK;
- (void)staticDataOK;
- (void)historyDataOK;

// Parsing methods
- (NSArray *)quotesParsing:(NSString *)quotes;
- (NSArray *)exchangesParsing:(NSString *)exchanges;
- (void)staticDataParsing:(NSString *)secOid;
- (void)historyDataParsing:(NSString *)secOid;

// Helper methods
- (NSString *)dataFromRHS:(NSString *)string;
- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings;
- (NSArray *)stripOffFirstElement:(NSArray *)array;
@end



@implementation mTraderCommunicator

static mTraderCommunicator *sharedCommunicator = nil;
@synthesize symbolsDelegate;
@synthesize statusDelegate = _statusDelegate;

@synthesize defaults = _defaults;
@synthesize blockBuffer = _blockBuffer;
@synthesize qFields = _qFields;
@synthesize currentLine = _currentLine;
@synthesize state, contentLength;

#pragma mark -
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

#pragma mark -
#pragma mark Initialization

- (id)init {
	self = [super init];
	if (self != nil) {
		_blockBuffer = nil;
		_qFields = nil;
		contentLength = 0;
		state = LOGIN;
		
		DataController *dataController = [DataController sharedManager];
		symbolsDelegate = dataController;
	}
	return self;
}


#pragma mark -
#pragma mark Communicator Delegate Methods

- (void)receivedDataBlock:(NSArray *)block {
	self.blockBuffer = [NSMutableArray arrayWithArray:block];
	[self stateMachine];
}

#pragma mark -
#pragma mark State Machine

-(void) stateMachine {
	while ([self.blockBuffer count] > 0) {
		switch (state) {
			case HEADER:
#if DEBUG_STATE
				NSLog(@"State: HEADER");
#endif
				[self headerParsing];
				break;
			case FIXEDLENGTH:
#if DEBUG_STATE
				NSLog(@"State: FIXEDLENGTH");
#endif
				[self fixedLength];
				break;
			case STATICRESPONSE:
#if DEBUG_STATE
				NSLog(@"State: STATICRESPONSE");
#endif
				[self staticResponse];
				break;
			case CHART:
#if DEBUG_STATE
				NSLog(@"State: CHART");
#endif
				[self chartHandling];
				break;
			case LOGIN:
#if DEBUG_STATE
				NSLog(@"State: LOGIN");
#endif
				[self loginHandling];
				break;
			case PREPROCESSING:
#if DEBUG_STATE
				NSLog(@"State: PREPROCESSING");
#endif
				[self preprocessing];
				break;
			case PROCESSING:
#if DEBUG_STATE
				NSLog(@"State: PROCESSING");
#endif
				[self processingLoop];
				break;
			case NEWSFEEDS:
#if DEBUG_STATE
				NSLog(@"State: NEWSFEEDS");
#endif
				[self newsListFeedsOK];
				break;
			case NEWSLIST:
#if DEBUG_STATE
				NSLog(@"State: NEWSLIST");
#endif
				[self newsListOK];
				break;
			case NEWSITEM:
#if DEBUG_STATE
				NSLog(@"State: NEWSITEM");
#endif
				[self newsBodyOK];
				break;
			case QUOTE:
#if DEBUG_STATE
				NSLog(@"State: QUOTE");
#endif
				[self quoteHandling];
				break;
			case SEARCHRESULTS:
#if DEBUG_STATE
				NSLog(@"State: SEARCHRESULTS");
#endif
				[self searchResultsHandling];
				 break;
			case ADDSEC:
#if DEBUG_STATE
				NSLog(@"State: ADDSEC");
#endif
				[self addSecurityOK];
				break;
			case REMSEC:
#if DEBUG_STATE
				NSLog(@"State: REMSEC");
#endif
				[self removeSecurityOK];
				break;
			case STATDATA:
#if DEBUG_STATE
				NSLog(@"State: STATDATA");
#endif
				[self staticDataOK];
				break;
			case HISTDATA:
#if DEBUG_STATE
				NSLog(@"State: HISTDATA");
#endif
				[self historyDataOK];
				break;
			default:
#if DEBUG_STATE
				NSLog(@"State: INVALID");
#endif
				break;
		}
	}
}

- (void)headerParsing {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
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
#if DEBUG
		NSLog(@"Fixed-length response didn't match advertised size: %d vs. %d", contentLength, bytes);
#endif
	}
	
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
	if ([string rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(loginFailed:)]) {
			[self.statusDelegate loginFailed:@"UsrPwd"];
		}
		state = LOGIN;
	} else if ([string rangeOfString:@"Request: login/failed.DeniedAccess"].location == 0) {
		if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(loginFailed:)]) {
			[self.statusDelegate loginFailed:@"DeniedAccess"];
		}
		state = LOGIN;
	} 
}

-(void) staticResponse {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
	if ([string rangeOfString:@"Request: login/OK"].location == 0) {
		if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(loginSuccess)]) {
			[self.statusDelegate loginSuccessful];
		}		
		state = PREPROCESSING;
	} else if ([string rangeOfString:@"Request: addSec/OK"].location == 0) {
		state = ADDSEC;
	} else if ([string rangeOfString:@"Request: remSec/OK"].location == 0) {
		state = REMSEC;
	} else if ([string rangeOfString:@"Request: incSearch/OK"].location == 0) {
		state = SEARCHRESULTS;
	} else if ([string rangeOfString:@"Request: incSearch/failed.NoHit"].location == 0) {
		[self searchNoHit];
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
	}
}

- (void)loginHandling {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
	if ([string rangeOfString:@"Request: login/OK"].location == 0) {
		if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(loginSuccessful)]) {
			[self.statusDelegate loginSuccessful];
		}
		loginStatusHasChanged = YES;
		
		state = PREPROCESSING;
	} else if ([string rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(loginFailed:)]) {
			[self.statusDelegate loginFailed:@"UsrPwd"];
		}
		loginStatusHasChanged = YES;
	}
}

/**
 * Read the login stream up to quotes, call settingsParsing to deal with it and
 * change state.
 */
- (void)preprocessing {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];

	if ([string rangeOfString:@"NewsFeeds:"].location == 0) {
		NSArray *feeds = [self exchangesParsing:string];
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(addNewsFeeds:)]) {
			[self.symbolsDelegate addNewsFeeds:feeds];
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
			if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(replaceAllSymbols:)]) {
				[self.symbolsDelegate replaceAllSymbols:symbols];
			}
		}
	} else if ([string rangeOfString:@"Exchanges:"].location == 0) {
		NSArray *exchanges = [self exchangesParsing:string];
		if (symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(addExchanges:)]) {
			[self.symbolsDelegate addExchanges:exchanges];
		}
	}
}

- (void)processingLoop {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
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
	NSString *string = [data string];
	
	if ([string rangeOfString:@"Quotes:"].location == 0) {
		NSArray *quotes = [self quotesParsing:string];
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(updateSymbols:)]) {
			[self.symbolsDelegate updateSymbols:quotes];
		}
		state = PROCESSING;
	} else if ([string rangeOfString:@"Kickout: 1"].location == 0) {
		state = KICKOUT;
		if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(kickedOut)]) {
			[self.statusDelegate kickedOut];
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
		NSString *string = [data string];
		
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
				string = [data string];
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

- (void)searchNoHit {
	if (symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(searchResults:)]) {
		[self.symbolsDelegate searchResults:nil];
	}
	state = PROCESSING;
}

- (void)searchResultsHandling {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
	NSMutableArray *results = [NSMutableArray array];
	while ([self.blockBuffer count] > 0) {	
		NSString *symbolsSansCRLF = [StringHelpers cleanString:string];
		NSArray *columns = [symbolsSansCRLF componentsSeparatedByString:@";"];
		NSString *feedTicker = [columns objectAtIndex:0];
		NSString *exchange = [columns objectAtIndex:1];
		NSString *description = nil;
		
		if ([columns count] > 3) {
			NSRange extraComponentsRange;
			extraComponentsRange.location = 2;
			extraComponentsRange.length = [columns count] - 2;
			columns = [columns subarrayWithRange:extraComponentsRange];
			
			description = [columns componentsJoinedByString:@";"];
		} else {
			description = [columns objectAtIndex:2];
		}
		
		columns = [NSArray arrayWithObjects:feedTicker, exchange, description, nil];		
		NSAssert([columns count] == 3, @"Search results contained more than three columns.");
		[results addObject:columns];
		
		data = [self.blockBuffer deQueue];
		string = [data string];
	}
	
	NSString *symbolsSansCRLF = [StringHelpers cleanString:string];
	NSArray *columns = [symbolsSansCRLF componentsSeparatedByString:@";"];
	NSString *feedTicker = [columns objectAtIndex:0];
	NSString *exchange = [columns objectAtIndex:1];
	NSString *description = nil;
	
	if ([columns count] > 3) {
		NSRange extraComponentsRange;
		extraComponentsRange.location = 2;
		extraComponentsRange.length = [columns count] - 2;
		columns = [columns subarrayWithRange:extraComponentsRange];
		description = [columns componentsJoinedByString:@";"];
	} else {
		description = [columns objectAtIndex:2];
	}
	
	columns = [NSArray arrayWithObjects:feedTicker, exchange, description, nil];
	NSAssert([columns count] == 3, @"Search results contained more than three columns.");
	[results addObject:columns];
	
	if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(searchResults:)]) {
		[self.symbolsDelegate searchResults:results];
	}
	
	state = PROCESSING;
}

- (void)addSecurityOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
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
	NSString *string = [data string];
	
	if ([string rangeOfString:@"SecOid:"].location == 0) {
		NSString *feedTicker = [StringHelpers cleanString:[[string componentsSeparatedByString:@":"] objectAtIndex:1]];
		if (self.symbolsDelegate && [self.symbolsDelegate respondsToSelector:@selector(removedSecurity:)]) {
			[self.symbolsDelegate removedSecurity:feedTicker];
		}
	}
	
	state = PROCESSING;
}

- (void)staticDataOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
	if ([string rangeOfString:@"SecOid:"].location == 0) {
		[self staticDataParsing:string];
	}
}

- (void)historyDataOK {
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
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
	NSString *string = [data string];
	
	if ([string rangeOfString:@"Staticdata:"].location == 0) {
		NSRange staticDataRange = [string rangeOfString:@"Staticdata: "];
		NSRange restOfTheDataRange;
		restOfTheDataRange.location = staticDataRange.length;
		restOfTheDataRange.length = [string length] - staticDataRange.length;
		NSString *staticDataString = [string substringWithRange:restOfTheDataRange];
		
		NSArray *staticDataRows = [StringHelpers cleanComponents:[staticDataString componentsSeparatedByString:@";"]];
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
	NSString *string = [data string];
	
	if ([string rangeOfString:@"FirstTrade:"].location == 0) {
		NSString *firstTrade = [StringHelpers cleanString:[[string componentsSeparatedByString:@":"] objectAtIndex:1]];
		[dataDictionary setObject:firstTrade forKey:@"firstTrade"];
	}
	
	data = [self.blockBuffer deQueue];
	string = [data string];
	
	if ([string rangeOfString:@"CountTrades:"].location == 0) {
		NSString *firstTrade = [StringHelpers cleanString:[[string componentsSeparatedByString:@":"] objectAtIndex:1]];
		[dataDictionary setObject:firstTrade forKey:@"countTrades"];
	}
	
	data = [self.blockBuffer deQueue];
	string = [data string];
	
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
	NSString *string = [data string];
	
	if ([string rangeOfString:@"News:"].location == 0) {
		NSString *newsArticles = [data string];
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
	NSData *data = [self.blockBuffer deQueue];
	NSString *string = [data string];
	
	if ([string rangeOfString:@"News:"].location == 0) {
		NSString *newsArticles = [data string];
		NSArray *colonSeparatedComponents = [newsArticles componentsSeparatedByString:@":"];
		colonSeparatedComponents = [self stripOffFirstElement:colonSeparatedComponents];
		newsArticles = [colonSeparatedComponents componentsJoinedByString:@":"];
		NSArray *newsArticlesArray = [StringHelpers cleanComponents:[newsArticles componentsSeparatedByString:@"|"]];
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
		NSString *string = [data string];
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

- (BOOL)loginStatusHasChanged {
	BOOL result = NO;
	if (loginStatusHasChanged) {
		result = YES;
		
		loginStatusHasChanged = NO;
	}
	return result;
}

#pragma mark -
#pragma mark mTrader Server Requests
/**
 * These are methods that format mTrader Server Messages
 *
 */

- (void)login {
	NSString *username = self.defaults.username;
	NSString *password = self.defaults.password;
	
	username = [StringHelpers cleanString:username];
	password = [StringHelpers cleanString:password];
	if (username != nil && password != nil && ![username isEqualToString:@""] && ![password isEqualToString:@""]) {
		NSString *version = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
		
		NSString *ActionLogin = @"Action: login";
		NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@/%@", username, password];
		NSString *Platform = [NSString stringWithFormat:@"Platform: %@ %@", [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion]];
		NSString *Client = @"Client: SEB mTrader";
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
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
		return;
	}
	NSString *ActionLogout = @"Action: Logout";
	NSArray *logoutArray = [NSArray arrayWithObjects:ActionLogout, nil];
	NSString *logoutString = [self arrayToFormattedString:logoutArray];
	[self.communicator writeString:logoutString];
}

- (void)addSecurity:(NSString *)tickerSymbol withMCode:(NSString *)mCode {
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
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
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
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
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
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
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO || self.qFields == nil) {
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
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
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
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
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
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
		return;
	}
	NSString *username = self.defaults.username;
	NSString *ActionNewsBody = @"Action: NewsBody";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *NewsID = [NSString stringWithFormat:@"NewsID: %@", newsId];
	
	NSArray *newsItemArray = [NSArray arrayWithObjects:ActionNewsBody, Authorization, NewsID, nil];
	NSString *newsItemRequestString = [self arrayToFormattedString:newsItemArray];
	
	[self.communicator writeString:newsItemRequestString];
}

- (void)newsListFeed:(NSString *)mCode {
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
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
	
	if (isLoggedIn == YES) {
		[self.communicator writeString:newsListFeedsString];
	}
}

- (void)symbolNewsForFeedTicker:(NSString *)feedTicker {
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
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

- (void)symbolSearch:(NSString *)symbol {
	mTraderServerMonitor *monitor = [mTraderServerMonitor sharedManager];
	BOOL isLoggedIn = [monitor loggedIn];
	if ( isLoggedIn == NO ) {
		return;
	}
	
	NSString *ActionIncSearch = @"Action: incSearch";
	NSString *Search = [NSString stringWithFormat:@"Search: %@", symbol];
	NSString *maxCount = @"MaxHits: 50";
	
	NSArray *symbolSearchRequest = [NSArray arrayWithObjects:ActionIncSearch, Search, maxCount, nil];
	NSString *symbolSearchString = [self arrayToFormattedString:symbolSearchRequest];
	
	[self.communicator writeString:symbolSearchString];		
}

#pragma mark -
#pragma mark Helper Methods
/**
 * Helper methods
 *
 */

-(NSString *)dataFromRHS:(NSString *)string {
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

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_qFields release];
	[_blockBuffer release];
	[_communicator release];
	
	[super dealloc];
}

@end
