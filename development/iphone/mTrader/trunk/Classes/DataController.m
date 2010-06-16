//
//  SymbolDataController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 1

#import "DataController.h"

#import "mTraderCommunicator.h"
#import "QFields.h"
#import "StringHelpers.h"

#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Trade.h"
#import "Chart.h"
#import "NewsFeed.h"
#import "NewsArticle.h"
#import "BidAsk.h"

static DataController *sharedDataController = nil;

@implementation DataController
@synthesize orderBookDelegate;
@synthesize tradesDelegate;
@synthesize searchDelegate;
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize managedObjectContext = _managedObjectContext;

- (id)init {
	self = [super init];
	if (self != nil) {
		_managedObjectContext = nil;
		_fetchedResultsController = nil;
	}
	return self;
}


#pragma mark Singleton Methods
/**
 * Methods for Singleton implementation
 *
 */
+ (DataController *)sharedManager {
	if (sharedDataController == nil) {
		sharedDataController = [[super allocWithZone:NULL] init];
	}
	return sharedDataController;
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

- (void)setManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	_managedObjectContext = [managedObjectContext retain];
	
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort(); // Fail
#endif
	}
}


#pragma mark -
#pragma mark Delegation
/**
 * Delegation
 */

- (void)addNewsFeeds:(NSArray *)feeds {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	feeds = [StringHelpers cleanComponents:feeds];
	
	NSString *allNewsLocalizedString = NSLocalizedString(@"AllNews", "All News Feeds Localization");
	NSString *feedString = [NSString stringWithFormat:@"0:AllNews:%@ [AllNews](S)", allNewsLocalizedString];
	NSArray *modifiedFeeds = [NSArray arrayWithObject:feedString];
	modifiedFeeds = [modifiedFeeds arrayByAddingObjectsFromArray:feeds];
	
	for (NSString *feed in modifiedFeeds) {
		NSArray *exchangeComponents = [feed componentsSeparatedByString:@":"];
		
		if ([exchangeComponents count] != 3) {
			NSLog(@"Exchange %@ rejected. Improper number of fields");
			continue;
		}
		
		NSString *feedNumber = [exchangeComponents objectAtIndex:0];
		NSString *mCode = [exchangeComponents objectAtIndex:1];
		
		NSString *description = [exchangeComponents objectAtIndex:2];
		
		// Separate the Description from the mCode
		NSRange leftBracketRange = [description rangeOfString:@"["];
		//NSRange rightBracketRange = [description rangeOfString:@"]"];
		NSRange leftParenthesisRange = [description rangeOfString:@"("];
		NSRange rightParenthesisRange = [description rangeOfString:@")"];
		
		NSRange typeRange;
		typeRange.location = leftParenthesisRange.location + 1;
		typeRange.length = rightParenthesisRange.location - typeRange.location;
		NSString *typeCode = [description substringWithRange:typeRange]; // (S) 
		
		NSRange descriptionRange;
		descriptionRange.location = 0;
		descriptionRange.length = leftBracketRange.location - 1;
		NSString *feedName = [description substringWithRange:descriptionRange]; // Oslo Stocks		
		
		NewsFeed *newsFeed = [self fetchNewsFeed:mCode];
		if (newsFeed == nil) {
			newsFeed = (NewsFeed *)[NSEntityDescription insertNewObjectForEntityForName:@"NewsFeed" inManagedObjectContext:self.managedObjectContext];
		}
		
		newsFeed.feedNumber = feedNumber;
		newsFeed.mCode = mCode;
		newsFeed.name = feedName;
		newsFeed.type = typeCode;
	}
	
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		// Handle the error
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
}

- (void)addExchanges:(NSArray *)exchanges {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	exchanges = [StringHelpers cleanComponents:exchanges];
	for (NSString *exchangeCode in exchanges) {
		NSArray *exchangeComponents = [exchangeCode componentsSeparatedByString:@":"];
		
		if ([exchangeComponents count] != 4) {
#if DEBUG
			NSLog(@"Exchange %@ rejected. Improper number of fields");
#endif
			continue;
		}
		
		NSString *feedNumber = [exchangeComponents objectAtIndex:0];
		NSString *mCode = [exchangeComponents objectAtIndex:1];
		
		NSString *description = [exchangeComponents objectAtIndex:2];
		NSString *decimals = [exchangeComponents objectAtIndex:3];
		
		// Separate the Description from the mCode
		NSRange leftBracketRange = [description rangeOfString:@"["];
		//NSRange rightBracketRange = [description rangeOfString:@"]"];
		NSRange leftParenthesisRange = [description rangeOfString:@"("];
		NSRange rightParenthesisRange = [description rangeOfString:@")"];
		
		NSRange typeRange;
		typeRange.location = leftParenthesisRange.location + 1;
		typeRange.length = rightParenthesisRange.location - typeRange.location;
		NSString *typeCode = [description substringWithRange:typeRange]; // (S) 
				
		NSRange descriptionRange;
		descriptionRange.location = 0;
		descriptionRange.length = leftBracketRange.location - 1;
		NSString *feedName = [description substringWithRange:descriptionRange]; // Oslo Stocks
		
		Feed *feed = [self fetchFeedByName:feedName];
		if (feed == nil) {
			feed = (Feed *)[NSEntityDescription insertNewObjectForEntityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
		}
		
		feed.feedNumber = [NSNumber numberWithInteger:[feedNumber integerValue]];
		feed.mCode = mCode;
		feed.feedName = feedName;
		feed.typeCode = typeCode;
		feed.decimals = [NSNumber numberWithInteger:[decimals integerValue]];
	}
	
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		// Handle the error
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
}

- (void)searchResults:(NSArray *)results {
	if (self.searchDelegate && [self.searchDelegate respondsToSelector:@selector(searchResultsUpdate:)]) {
		[self.searchDelegate searchResultsUpdate:results];
	}
}

- (void)replaceAllSymbols:(NSString *)symbols {
	[self deleteAllSymbols];
	[self addSymbols:symbols];
}

- (void)addSymbols:(NSString *)symbols {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	//static NSInteger FEED_TICKER = 0;
	static NSInteger TICKER_SYMBOL = 1;
	static NSInteger COMPANY_NAME = 2;
	static NSInteger EXCHANGE_CODE = 3;
	static NSInteger TYPE = 4;
	static NSInteger ORDER_BOOK = 5;
	static NSInteger ISIN = 6;
	//static NSInteger EXCHANGE_NUMBER = 7;
	static NSInteger FIELD_COUNT = 8;
	
	// insert the objects
	NSInteger index = 0;
	NSArray *rows = [symbols componentsSeparatedByString:@":"];	
	for (NSString *row in rows) {
		NSArray *stockComponents = [row componentsSeparatedByString:@";"];
		if ([stockComponents count] != FIELD_COUNT) {
#if DEBUG
			NSLog(@"Adding symbol string %@ failed. Wrong number of fields.", row);
#endif
			continue;
		}
		stockComponents = [StringHelpers cleanComponents:stockComponents];
		//NSString *ticker = [feedTickerComponents objectAtIndex:1];
		NSString *tickerSymbol = [stockComponents objectAtIndex:TICKER_SYMBOL];
		NSString *companyName = [stockComponents objectAtIndex:COMPANY_NAME];
		NSString *exchangeCode = [stockComponents objectAtIndex:EXCHANGE_CODE];
		NSString *orderBook = [stockComponents objectAtIndex:ORDER_BOOK];
		NSString *type = [stockComponents objectAtIndex:TYPE];
		NSString *isin = [stockComponents objectAtIndex:ISIN];
		
		// Separate the Description from the mCode
		NSRange leftBracketRange = [exchangeCode rangeOfString:@"["];
		NSRange rightBracketRange = [exchangeCode rangeOfString:@"]"];
		
		NSRange mCodeRange;
		mCodeRange.location = leftBracketRange.location + 1;
		mCodeRange.length = rightBracketRange.location - mCodeRange.location;
		NSString *mCode = [exchangeCode substringWithRange:mCodeRange]; // OSS

		
		// Prevent double insertions
		Feed *feed = [self fetchFeed:mCode];		
		Symbol *symbol = [self fetchSymbol:tickerSymbol withFeed:mCode]; 
		if (symbol == nil) {
			symbol = (Symbol *)[NSEntityDescription insertNewObjectForEntityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
			symbol.tickerSymbol = tickerSymbol;
			
			symbol.index = [NSNumber numberWithInteger:index];
			symbol.companyName = companyName;
			symbol.country = nil;
			symbol.currency = nil;
			symbol.orderBook = orderBook;
			if ([type isEqualToString:@"1"]) {
				symbol.type = @"Stock";
			} else if ([type isEqualToString:@"2"]) {
				symbol.type = @"Index";
			} else if ([type isEqualToString:@"3"]) {
				symbol.type = @"Exchange Rate";
			} else {
				symbol.type = type;
			}
			symbol.isin = isin;
			symbol.symbolDynamicData = (SymbolDynamicData *)[NSEntityDescription insertNewObjectForEntityForName:@"SymbolDynamicData" inManagedObjectContext:self.managedObjectContext];
			
			[feed addSymbolsObject:symbol];
			
			NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", symbol.feed.feedNumber, symbol.tickerSymbol];
			[[mTraderCommunicator sharedManager] staticDataForFeedTicker:feedTicker];
		}
		
		index++;
	}
	// save the objects
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		// Handle the error
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();  // Fail
#endif
	}
}

/**
 * This method should receive a list of symbols that have been updated and should
 * update any rows necessary.
 */
- (void)updateSymbols:(NSArray *)updates {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	static NSTimeZone *timeZone = nil;
	if (timeZone == nil) {
		timeZone = [NSTimeZone timeZoneWithName:@"CET"];
	}
	
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setTimeZone:timeZone];
		[dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
	}
	
	static NSDateFormatter *yearFormatter = nil;
	if (yearFormatter == nil) {
		yearFormatter = [[NSDateFormatter alloc] init];
		[yearFormatter setDateFormat:@"yyyy-MM-dd"];
	}
	
	NSDate *today = [NSDate date];
	NSString *todayString = [yearFormatter stringFromDate:today];
	
	for (NSDictionary *update in updates) {
		NSString *feedTicker = [update objectForKey:@"feedTicker"];
		NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
		NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
		NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
		
		NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
		NSEntityDescription *entity = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
		[request setEntity:entity];
		
		NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed.feedNumber=%@) AND (tickerSymbol=%@)", feedNumber, tickerSymbol];
		[request setPredicate:predicate];
		
		NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
		[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
		[sortDescriptor release];
		
		NSError *error = nil;
		NSArray *resultSetArray = [self.managedObjectContext executeFetchRequest:request error:&error];
		if (resultSetArray == nil)
		{
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
			abort();
#endif
			continue;
		}
		
		if ([resultSetArray count] == 0) {
			NSLog(@"Symbol Update failed for %@. Unable to locate symbol.", feedTicker);
			continue;
		}
		
		Symbol *symbol = [resultSetArray objectAtIndex:0];
		resultSetArray = nil;
		
		// timestamp
		NSString *timeStampKey = [NSString stringWithFormat:@"%d", TIMESTAMP];
		if ([update objectForKey:timeStampKey]) {
			NSString *timeStamp = [update valueForKey:timeStampKey];
			if ([timeStamp isEqualToString:@"--"] == YES || [timeStamp isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.lastTradeTime = nil;
			} else if ([timeStamp isEqualToString:@""] == NO) {
				NSString *dateFormattedString = [NSString stringWithFormat:@"%@ %@", todayString, timeStamp];
				NSDate *lastTradeTime = [dateFormatter dateFromString:dateFormattedString];
				symbol.symbolDynamicData.lastTradeTime = lastTradeTime;
			}
		}
		
		// last trade
		NSString *lastTradeKey = [NSString stringWithFormat:@"%d", LAST_TRADE];
		if ([update objectForKey:lastTradeKey]) {
			NSString *lastTrade = [update valueForKey:lastTradeKey];
			if ([lastTrade isEqualToString:@"--"] == YES || [lastTrade isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.lastTrade = nil;
			} else if ([lastTrade isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.lastTrade = [NSNumber numberWithDouble:[lastTrade doubleValue]];
			}
		}
		
		// change
		NSString *changeKey = [NSString stringWithFormat:@"%d", CHANGE];
		if ([update objectForKey:changeKey]) {
			NSString *change = [update valueForKey:changeKey];
			if ([change isEqualToString:@"--"] == YES || [change isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.change = nil;
			} else if ([change isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.change = [NSNumber numberWithDouble:[change doubleValue]];
			}
		}
		
		// change percent
		NSString *changePercentKey = [NSString stringWithFormat:@"%d", CHANGE_PERCENT];
		if ([update objectForKey:changePercentKey]) {
			NSString *changePercent = [update valueForKey:changePercentKey];
			if ([changePercent isEqualToString:@"--"] == YES || [changePercent isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.changePercent = nil;
			} else if ([changePercent isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.changePercent = [NSNumber numberWithDouble:([changePercent doubleValue]/100.0)];
			}
		}
		
		// change arrow
		NSString *changeArrowKey = [NSString stringWithFormat:@"%d", CHANGE_ARROW];
		if ([update objectForKey:changeArrowKey]) {
			NSString *changeArrow = [update valueForKey:changeArrowKey];
			if ([changeArrow isEqualToString:@"--"] == YES || [changeArrow isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.changeArrow = nil;
			} else if ([changeArrow isEqualToString:@""] == NO) {
				NSNumber *changeNumber = [NSNumber numberWithInteger:[changeArrow integerValue]];
				symbol.symbolDynamicData.changeArrow = changeNumber;
#if DEBUG
				if ([changeArrow isEqualToString:@"1"] || [changeArrow isEqualToString:@"3"]) {
					NSLog(@">>>Flash ticker symbol %@<<<", feedTicker);
				}
#endif
			}
		}
		
		// bid price
		NSString *bidPriceKey = [NSString stringWithFormat:@"%d", BID_PRICE];
		if ([update objectForKey:bidPriceKey]) {
			NSString *bidPrice = [update valueForKey:bidPriceKey];
			if ([bidPrice isEqualToString:@"--"] == YES || [bidPrice isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.bidPrice = nil;
			} else if ([bidPrice isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.bidPrice = [NSNumber numberWithDouble:[bidPrice doubleValue]];
			}
		}
		
		// ask price
		NSString *askPriceKey = [NSString stringWithFormat:@"%d", ASK_PRICE];
		if ([update objectForKey:askPriceKey]) {
			NSString *askPrice = [update valueForKey:askPriceKey];
			if ([askPrice isEqualToString:@"--"] == YES || [askPrice isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.askPrice = nil;
			} else if ([askPrice isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.askPrice = [NSNumber numberWithDouble:[askPrice doubleValue]];
			}
		}
		
		// ask volume
		NSString *askVolumeKey = [NSString stringWithFormat:@"%d", ASK_VOLUME];
		if ([update objectForKey:askVolumeKey]) {
			NSString *askVolume = [update valueForKey:askVolumeKey];
			if ([askVolume isEqualToString:@"--"] == YES || [askVolume isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.askVolume = nil;
			} else if ([askVolume isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.askVolume = askVolume;
			}
		}
		
		// bid volume
		NSString *bidVolumeKey = [NSString stringWithFormat:@"%d", BID_VOLUME];
		if ([update objectForKey:bidVolumeKey]) {
			NSString *bidVolume = [update valueForKey:bidVolumeKey];
			if ([bidVolume isEqualToString:@"--"] == YES || [bidVolume isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.bidVolume = nil;
			} else if ([bidVolume isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.bidVolume = bidVolume;
			}
		}
		
		// high
		NSString *highKey = [NSString stringWithFormat:@"%d", HIGH];
		if ([update objectForKey:highKey]) {
			NSString *high = [update valueForKey:highKey];
			if ([high isEqualToString:@"--"] == YES || [high isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.high = nil;
			} else if ([high isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.high = [NSNumber numberWithDouble:[high doubleValue]];
			}
		}
		
		// low
		NSString *lowKey = [NSString stringWithFormat:@"%d", LOW];
		if ([update objectForKey:lowKey]) {
			NSString *low = [update valueForKey:lowKey];
			if ([low isEqualToString:@"--"] == YES || [low isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.low = nil;
			} else if ([low isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.low = [NSNumber numberWithDouble:[low doubleValue]];
			}
		}
		
		// open
		NSString *openKey = [NSString stringWithFormat:@"%d", OPEN];
		if ([update objectForKey:openKey]) {
			NSString *open = [update valueForKey:openKey];
			if ([open isEqualToString:@"--"] == YES || [open isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.open = nil;
			} else if ([open isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.open = [NSNumber numberWithDouble:[open doubleValue]];
			}
		}
		
		// volume
		NSString *volumeKey = [NSString stringWithFormat:@"%d", VOLUME];
		if ([update objectForKey:volumeKey]) {
			NSString *volume = [update valueForKey:volumeKey];
			if ([volume isEqualToString:@"--"] == YES || [volume isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.volume = nil;
			} else if ([volume isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.volume = volume;
			}
		}
		
		// orderBook
		NSString *orderBookKey = [NSString stringWithFormat:@"%d", ORDERBOOK];
		if ([update objectForKey:orderBookKey]) {
			// Bid then Ask -
			NSString *orderBookString = [update valueForKey:orderBookKey];
			NSArray *orderBook = [orderBookString componentsSeparatedByString:@"/"];
			orderBook = [StringHelpers cleanComponents:orderBook];
			
			if ([orderBook count] != 2) {
				NSLog(@"Orderbook has the wrong number of fields. %@", orderBookString);
			}
			
			NSString *bidsString = [orderBook objectAtIndex:0];
			NSString *asksString = [orderBook objectAtIndex:1];
			
			NSArray *bids = [bidsString componentsSeparatedByString:@"#"];
			NSArray *asks = [asksString componentsSeparatedByString:@"#"];
			
			for (int i = 0; i < [bids count]; i++) {
				NSString *bid = [bids objectAtIndex:i]; 
				if (![bid isEqualToString:@""]) {
					BidAsk *bidAsk = [self fetchBidAskForFeedTicker:feedTicker atIndex:i];
					if (bidAsk == nil) {
						bidAsk = [NSEntityDescription insertNewObjectForEntityForName:@"BidAsk" inManagedObjectContext:self.managedObjectContext];
						[symbol addBidsAsksObject:bidAsk];
						bidAsk.symbol = symbol;
						bidAsk.index = [NSNumber numberWithInteger:i];
					}
					
					NSArray *pieces = [bid componentsSeparatedByString:@"\\"];
					
					NSString *value = [pieces objectAtIndex:0];
					NSString *size = [pieces objectAtIndex:1];
					
					bidAsk.bidPrice = [NSNumber numberWithDouble:[value doubleValue]];
					bidAsk.bidSize = [NSNumber numberWithDouble:[size doubleValue]];
				}
			}
			
			NSArray *bidsToCalculate = [DataController fetchBidAsksForSymbol:symbol.tickerSymbol withFeedNumber:symbol.feed.feedNumber inManagedObjectContext:self.managedObjectContext];

			bidsToCalculate = [bidsToCalculate sortedArrayUsingSelector:@selector(compareBidSize:)];
			bidsToCalculate = [[bidsToCalculate reverseObjectEnumerator] allObjects];
			NSUInteger bidLargest;
			for (int i = 0; i < [bidsToCalculate count]; i++) {
				BidAsk *bidAsk = [bidsToCalculate objectAtIndex:i];
				if (i == 0) {
					bidLargest = [bidAsk.bidSize integerValue];
					bidAsk.bidPercent = [NSNumber numberWithFloat:1.0];
				} else {
					bidAsk.bidPercent = [NSNumber numberWithFloat:([bidAsk.bidSize doubleValue] / bidLargest)];
				}
			}
			
			for (int i = 0; i < [asks count]; i++) {
				NSString *ask = [asks objectAtIndex:i];
				if (![ask isEqualToString:@""] && ![ask isEqualToString:@"/"]) {
					BidAsk *bidAsk = [self fetchBidAskForFeedTicker:feedTicker atIndex:i];
					if (bidAsk == nil) {
						bidAsk = [NSEntityDescription insertNewObjectForEntityForName:@"BidAsk" inManagedObjectContext:self.managedObjectContext];
						[symbol addBidsAsksObject:bidAsk];
						bidAsk.symbol = symbol;
						bidAsk.index = [NSNumber numberWithInteger:i];
					}			
					
					NSArray *pieces = [ask componentsSeparatedByString:@"\\"];
					
					NSString *value = [pieces objectAtIndex:0];
					NSString *size = [pieces objectAtIndex:1];
								
					bidAsk.askPrice = [NSNumber numberWithDouble:[value doubleValue]];
					bidAsk.askSize = [NSNumber numberWithDouble:[size doubleValue]];
				}
			}
			
			NSArray *asksToCalculate = [DataController fetchBidAsksForSymbol:symbol.tickerSymbol withFeedNumber:symbol.feed.feedNumber inManagedObjectContext:self.managedObjectContext];

			asksToCalculate = [asksToCalculate sortedArrayUsingSelector:@selector(compareAskSize:)];
			asksToCalculate = [[asksToCalculate reverseObjectEnumerator] allObjects];
			CGFloat askLargest;
			for (int i = 0; i < [asksToCalculate count]; i++) {
				BidAsk *bidAsk = [asksToCalculate objectAtIndex:i];
				if (i == 0) {
					askLargest = [bidAsk.askSize integerValue];
					bidAsk.askPercent = [NSNumber numberWithFloat:1.0];
				} else {
					bidAsk.askPercent = [NSNumber numberWithFloat:([bidAsk.askSize doubleValue] / askLargest)];
				}
			}
			if ((self.orderBookDelegate != nil) && [self.orderBookDelegate respondsToSelector:@selector(updateOrderBook)]) {
				[self.orderBookDelegate updateOrderBook];
			}
		}
	}
		
	NSError *saveError;
	if (![self.managedObjectContext save:&saveError]) {
		NSLog(@"Unresolved error %@, %@", saveError, [saveError userInfo]);
#if DEBUG
		abort();
#endif
	}
}

- (void)staticUpdates:(NSDictionary *)updateDictionary {
	NSArray *feedTickerComponents = [[updateDictionary objectForKey:@"feedTicker"] componentsSeparatedByString:@"/"];
	NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	
	Symbol *symbol = [self fetchSymbol:tickerSymbol withFeedNumber:feedNumber];
	if ([updateDictionary objectForKey:@"Bid"]) {
		symbol.symbolDynamicData.bidPrice = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Bid"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"B Size"]) {
		symbol.symbolDynamicData.bidSize = [updateDictionary objectForKey:@"B Size"];
	}
	if ([updateDictionary objectForKey:@"Ask"]) { 
		symbol.symbolDynamicData.askPrice = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Ask"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"A Size"]) { 
		symbol.symbolDynamicData.askSize = [updateDictionary objectForKey:@"A Size"];
	}
	if ([updateDictionary objectForKey:@"Pr Cls"]) { 
		symbol.symbolDynamicData.previousClose = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Pr Cls"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"Open"]) { 
		symbol.symbolDynamicData.open = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Open"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"High"]) { 
		symbol.symbolDynamicData.high = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"High"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"Low"]) { 
		symbol.symbolDynamicData.low = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Low"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"Last"]) { 
		symbol.symbolDynamicData.lastTrade = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Last"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"L +/-"]) { 
		symbol.symbolDynamicData.change = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"L +/-"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"L +/-%"]) {
		symbol.symbolDynamicData.changePercent = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"L +/-%"] doubleValue]/100.0f];
	}
	if ([updateDictionary objectForKey:@"O +/-"]) { 
		symbol.symbolDynamicData.openChange = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"O +/-"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"O +/-%"]) {
		symbol.symbolDynamicData.openPercentChange = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"O +/-%"] doubleValue]/100.0f];
	}
	if ([updateDictionary objectForKey:@"Trades"]) {
		symbol.symbolDynamicData.trades = [updateDictionary objectForKey:@"Trades"];
	}	
	if ([updateDictionary objectForKey:@"Volume"]) {
		symbol.symbolDynamicData.volume = [updateDictionary objectForKey:@"Volume"];
	}
	if ([updateDictionary objectForKey:@"Turnover"]) {
		symbol.symbolDynamicData.turnover = [updateDictionary objectForKey:@"Turnover"];
	}
	if ([updateDictionary objectForKey:@"OnVolume"]) {
		symbol.symbolDynamicData.onVolume = [updateDictionary objectForKey:@"OnVolume"];
	}
	if ([updateDictionary objectForKey:@"OnValue"]) {
		symbol.symbolDynamicData.onValue = [updateDictionary objectForKey:@"OnValue"];
	}
	if ([updateDictionary objectForKey:@"VWAP"]) { 
		symbol.symbolDynamicData.VWAP = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"VWAP"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"AvgVol"]) {
		symbol.symbolDynamicData.averageVolume = [updateDictionary objectForKey:@"AvgVol"];
	}
	if ([updateDictionary objectForKey:@"AvgVal"]) { 
		symbol.symbolDynamicData.averageValue = [updateDictionary objectForKey:@"AvgVal"];
	}
	if ([updateDictionary objectForKey:@"Status"]) { 
		symbol.symbolDynamicData.tradingStatus = [updateDictionary objectForKey:@"Status"];
	}
	if ([updateDictionary objectForKey:@"B Lot"]) {
		symbol.symbolDynamicData.buyLot = [updateDictionary objectForKey:@"B Lot"];
	}
	if ([updateDictionary objectForKey:@"BLValue"]) { 
		symbol.symbolDynamicData.buyLotValue = [updateDictionary objectForKey:@"BLValue"];
	}
	if ([updateDictionary objectForKey:@"Shares"]) {
		symbol.symbolDynamicData.outstandingShares = [updateDictionary objectForKey:@"Shares"];
	}
	if ([updateDictionary objectForKey:@"M Cap"]) {
		symbol.symbolDynamicData.marketCapitalization = [updateDictionary objectForKey:@"M Cap"];
	}
	if ([updateDictionary objectForKey:@"Exchange"]) {
		//
	}
	if ([updateDictionary objectForKey:@"Country"]) {
		symbol.country = [updateDictionary objectForKey:@"Country"];
	}
	if ([updateDictionary objectForKey:@"Description"]) {
		//
	}
	if ([updateDictionary objectForKey:@"Symbol"]) {
		//
	}
	if ([updateDictionary objectForKey:@"ISIN"]) { 
		//
	}
	if ([updateDictionary objectForKey:@"Currency"]) { 
		symbol.currency = [updateDictionary objectForKey:@"Currency"];
	}
	if ([updateDictionary objectForKey:@"Segment"]) { 
		symbol.symbolDynamicData.segment = [updateDictionary objectForKey:@"Segment"];
	}
	if ([updateDictionary objectForKey:@"Dividend"]) { 
		symbol.symbolDynamicData.dividend = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Dividend"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"DivDate"]) { 
		symbol.symbolDynamicData.dividendDate = [updateDictionary objectForKey:@"DivDate"];
	}
	
}

- (void)chartUpdate:(NSDictionary *)chartData {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSArray *feedTickerComponents = [[chartData objectForKey:@"feedTicker"] componentsSeparatedByString:@"/"];
	NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	
	Symbol *symbol = [self fetchSymbol:tickerSymbol withFeedNumber:feedNumber];
	Chart *chart = [self fetchChart:tickerSymbol withFeedNumber:feedNumber];
	if (chart == nil) {
		chart = (Chart *)[NSEntityDescription insertNewObjectForEntityForName:@"Chart" inManagedObjectContext:self.managedObjectContext];
	}
	
	chart.height = [chartData objectForKey:@"height"];
	chart.width = [chartData objectForKey:@"width"];
	chart.size = [chartData objectForKey:@"size"];
	chart.type = [chartData objectForKey:@"type"];
	NSData *data = [chartData objectForKey:@"data"];
	chart.data = data;
	symbol.chart = chart;
	chart.symbol = symbol;
}

-(void) newsListFeedsUpdates:(NSArray *)newsList {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	static NSTimeZone *timeZone = nil;
	if (timeZone == nil) {
		timeZone = [NSTimeZone timeZoneWithName:@"CET"];
	}
	
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setTimeZone:timeZone];
		[dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
	}
	
	static NSDateFormatter *yearFormatter = nil;
	if (yearFormatter == nil) {
		yearFormatter = [[NSDateFormatter alloc] init];
		[yearFormatter setDateFormat:@"yyyy"];
	}
		
	NSDate *today = [NSDate date];
	NSString *year = [yearFormatter stringFromDate:today];
	
	for (NSString *news in newsList) {
		NSArray *components = [news componentsSeparatedByString:@";"];
		components = [StringHelpers cleanComponents:components];
		if ([components count] >= 4) {
			NSString *feedArticle = [components objectAtIndex:0];
			NSString *flag = [components objectAtIndex:1];
			
			NSString *month = nil;
			NSString *day = nil;
			NSString *date = [components objectAtIndex:2];
			if (![date isEqualToString:@""]) {
				NSArray *dateComponents = [date componentsSeparatedByString:@"."];
				month = [dateComponents objectAtIndex:1];
				day = [dateComponents objectAtIndex:0];
			}
			
			NSString *time = [components objectAtIndex:3];
			NSString *headline = [components objectAtIndex:4];
			
			NSArray *feedArticleComponents = [feedArticle componentsSeparatedByString:@"/"];			
			NSString *feedNumber = [feedArticleComponents objectAtIndex:0];
			NSString *articleNumber = [feedArticleComponents objectAtIndex:1];
			
			NSString *formattedDateString = [NSString stringWithFormat:@"%@-%@-%@ %@:00", year, month, day, time];
			NSDate *properDate = [dateFormatter dateFromString:formattedDateString];
						
			NewsArticle *article = [self fetchNewsArticle:articleNumber withFeed:feedNumber];
			if (article == nil) {
				NewsFeed *feed = [self fetchNewsFeedWithNumber:feedNumber];
				if (feed == nil) {
					continue;
				}
				
				article = (NewsArticle *)[NSEntityDescription insertNewObjectForEntityForName:@"NewsArticle" inManagedObjectContext:self.managedObjectContext];
				article.newsFeed = feed;
				[feed addNewsArticlesObject:article];
								
				article.articleNumber = articleNumber;
				article.flag = flag;
								
				article.date = properDate;
				article.headline = headline;
			}
			
		}
	}
	
	[self maxNewsArticles:250];
}

- (void)newsItemUpdate:(NSArray *)newsItemContents {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	newsItemContents = [StringHelpers cleanComponents:newsItemContents];
	NSString *feedArticle = [newsItemContents objectAtIndex:0];
	NSString *body = [newsItemContents objectAtIndex:4];
	
	NSArray *feedArticleComponents = [feedArticle componentsSeparatedByString:@"/"];
	NSString *feedNumber = [feedArticleComponents objectAtIndex:0];
	NSString *articleNumber = [feedArticleComponents objectAtIndex:1];
	
	body = [body stringByReplacingOccurrencesOfString:@"||" withString:@"\n"];
	body = [StringHelpers cleanString:body];

	NewsArticle *article = [self fetchNewsArticle:articleNumber withFeed:feedNumber];
	if (article == nil) {
		NewsFeed *feed = [self fetchNewsFeedWithNumber:feedNumber];
		article = (NewsArticle *)[NSEntityDescription insertNewObjectForEntityForName:@"NewsArticle" inManagedObjectContext:self.managedObjectContext];
		article.newsFeed = feed;
		[feed addNewsArticlesObject:article];
		article.articleNumber = articleNumber;
	}
	
	article.body = body;
}

- (void)tradesUpdate:(NSDictionary *)updateDictionary {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	// TPVAY
	// Time, Price, Volume, Buy/Seller, Type
	
	NSString *feedTicker = [updateDictionary objectForKey:@"feedTicker"];

	// Delete all trades
	[self deleteTradesForFeedTicker:feedTicker];

	NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
	NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	
	Symbol *symbol = [self fetchSymbol:tickerSymbol withFeedNumber:feedNumber];
	
	NSString *tradesString = [updateDictionary objectForKey:@"trades"];
	NSArray *tradesComponents = [tradesString componentsSeparatedByString:@"|"];
	
	for (int i = 0; i < [tradesComponents count]; i++) {
		NSString *tradeItem = [tradesComponents objectAtIndex:i];
		if ([tradeItem isEqualToString:@""]) {
			continue;
		}
		
		Trade *trade = [self fetchTradeForSymbol:feedTicker atIndex:i];
		if (trade == nil) {
			trade = (Trade *)[NSEntityDescription insertNewObjectForEntityForName:@"Trade" inManagedObjectContext:self.managedObjectContext];
			
			NSArray *parts = [tradeItem componentsSeparatedByString:@";"];
			
			NSInteger partCount = [parts count];
			if (partCount > 0) {
				NSString *time = [parts objectAtIndex:0];
				trade.time = time;
			}
			
			if (partCount > 1) {
				NSString *price = [parts objectAtIndex:1];
				trade.price = price;
			}
			
			if (partCount > 2) {
				NSString *volume = [parts objectAtIndex:2];
				trade.volume = volume;
			}
			
			if (partCount > 3) {
				NSString *buyerSeller = [parts objectAtIndex:3];
				NSArray *buyerSellerComponents = [buyerSeller componentsSeparatedByString:@"/"];
				trade.buyer = [buyerSellerComponents objectAtIndex:0];
				trade.seller = [buyerSellerComponents objectAtIndex:1];
			}
			
			if (partCount > 4) {
				NSString *type = [parts objectAtIndex:4];
				trade.type = type;
			}
			
			trade.index = [NSNumber numberWithInteger:i];
			
			trade.symbol = symbol;
			[symbol addTradesObject:trade];
		}
	}
	
	if (self.tradesDelegate && [self.tradesDelegate respondsToSelector:@selector(updateTrades)]) {
		[self.tradesDelegate updateTrades];
	}
}

- (void)failedToAddAlreadyExists {
	NSString *alertTitle = @"Add Security Failed";
	NSString *alertMessage = @"The ticker symbol you requested is already in your list.";
	NSString *alertCancel = @"Dismiss";
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertTitle message:alertMessage delegate:nil cancelButtonTitle:alertCancel otherButtonTitles:nil];
	[alertView show];
	[alertView release];
}

- (void)failedToAddNoSuchSecurity {
	NSString *alertTitle = @"Add Security Failed";
	NSString *alertMessage = @"The ticker symbol you requested was not found.";
	NSString *alertCancel = @"Dismiss";
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertTitle message:alertMessage delegate:nil cancelButtonTitle:alertCancel otherButtonTitles:nil];
	[alertView show];
	[alertView release];
}

- (Trade *)fetchTradeForSymbol:(NSString *)feedTicker atIndex:(NSUInteger)index {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
	NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"Trade" inManagedObjectContext:self.managedObjectContext];
	[request setEntity:entity];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(symbol.feed.feedNumber=%@) AND (symbol.tickerSymbol=%@) AND (index=%@)", feedNumber, tickerSymbol, [NSNumber numberWithInteger:index]];
	[request setPredicate:predicate];
	
	NSSortDescriptor *descriptor = [[NSSortDescriptor alloc] initWithKey:@"index" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:descriptor]];
	[descriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (BidAsk *)fetchBidAskForFeedTicker:(NSString *)feedTicker atIndex:(NSUInteger)index {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
	NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	
	NSFetchRequest *bidAskRequest = [[[NSFetchRequest alloc] init] autorelease];
	NSEntityDescription *bidAskEntity = [NSEntityDescription entityForName:@"BidAsk" inManagedObjectContext:self.managedObjectContext];
	[bidAskRequest setEntity:bidAskEntity];
	
	NSPredicate *bidAskPredicate = [NSPredicate predicateWithFormat:@"(symbol.feed.feedNumber=%@) AND (symbol.tickerSymbol=%@) AND (index=%@)", feedNumber, tickerSymbol, [NSNumber numberWithInteger:index]];
	[bidAskRequest setPredicate:bidAskPredicate];
	
	NSError *bidAskError = nil;
	NSArray *bidAskArray = [self.managedObjectContext executeFetchRequest:bidAskRequest error:&bidAskError];
	if (bidAskArray == nil)	{
		NSLog(@"Unresolved error %@, %@", bidAskError, [bidAskError userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([bidAskArray count] == 1) {
		return [bidAskArray objectAtIndex:0];
	} else {
		return nil;
	}
}

- (NewsFeed *)fetchNewsFeed:(NSString *)mCode {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"NewsFeed" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(mCode=%@)", mCode];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"mCode" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (NewsFeed *)fetchNewsFeedWithNumber:(NSString *)feedNumber {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"NewsFeed" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feedNumber=%@)", feedNumber];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"feedNumber" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (void)maxNewsArticles:(NSInteger)max {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"NewsArticle" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	[request setIncludesPropertyValues:NO];
	[request setIncludesSubentities:NO];
		
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"date" ascending:NO];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil) {
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	for (int i = max; i < [array count]; i++) {
		NewsArticle *newsArticle = [array objectAtIndex:i];
		[self.managedObjectContext deleteObject:newsArticle];
	}
}

- (void)deleteTradesForFeedTicker:(NSString *)feedTicker {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
	NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Trade" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(symbol.feed.feedNumber=%@) AND (symbol.tickerSymbol=%@)", feedNumber, tickerSymbol];
	[request setPredicate:predicate];
	
	[request setIncludesPropertyValues:NO];
	[request setIncludesSubentities:NO];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil) {
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	for (Trade *trade in array) {
		[self.managedObjectContext deleteObject:trade];
	}	
}

- (void)deleteAllNews {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"NewsArticle" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	
	[request setIncludesPropertyValues:NO];
	[request setIncludesSubentities:NO];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil) {
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	for (NewsArticle *newsArticle in array) {
		[self.managedObjectContext deleteObject:newsArticle];
	}	
}

- (void)deleteAllBidsAsks {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"BidAsk" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	[request setIncludesPropertyValues:NO];
	[request setIncludesSubentities:NO];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil) {
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	for (NewsArticle *newsArticle in array) {
		[self.managedObjectContext deleteObject:newsArticle];
	}	
}

- (NewsArticle *)fetchNewsArticle:(NSString *)articleNumber withFeed:(NSString *)feedNumber {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"NewsArticle" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(newsFeed.feedNumber=%@) AND (articleNumber=%@)", feedNumber, articleNumber];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"articleNumber" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
	
}

- (Feed *)fetchFeed:(NSString *)mCode {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(mCode=%@)", mCode];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"mCode" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (Feed *)fetchFeedByNumber:(NSString *)feedNumber {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feedNumber=%@)", feedNumber];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"feedNumber" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}	
	
}

- (Feed *)fetchFeedByName:(NSString *)feedName {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feedName=%@)", feedName];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"feedName" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}	
}

- (void)deleteAllSymbols {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	[request setIncludesPropertyValues:NO];
	[request setIncludesSubentities:NO];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil) {
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	for (Symbol *symbol in array) {
		[self.managedObjectContext deleteObject:symbol];
	}
}

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed.feedNumber=%@) AND (tickerSymbol=%@)", feedNumber, tickerSymbol];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (Chart *)fetchChart:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Chart" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(symbol.feed.feedNumber=%@) AND (symbol.tickerSymbol=%@)", feedNumber, tickerSymbol];
	[request setPredicate:predicate];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (NSArray *)fetchAllSymbols {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] >= 1) {
		return array;
	} else {
		return nil;
	}
}

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed.mCode=%@) AND (tickerSymbol=%@)", mCode, tickerSymbol];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (void)removeSymbol:(Symbol *)symbol {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", symbol.feed.feedNumber, symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] removeSecurity:feedTicker];
	
	[self.managedObjectContext deleteObject:symbol];
}

+ (NSArray *)fetchBidAsksForSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber inManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	NSAssert(managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"BidAsk" inManagedObjectContext:managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(symbol.feed.feedNumber=%@) AND (symbol.tickerSymbol=%@)", feedNumber, tickerSymbol];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"index" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] > 0) {
		return array;
	} else {
		return nil;
	}
}

+ (NSArray *)fetchTradesForSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber inManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	NSAssert(managedObjectContext != nil, @"NSManagedObjectContext is nil");

	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Trade" inManagedObjectContext:managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(symbol.feed.feedNumber=%@) AND (symbol.tickerSymbol=%@)", feedNumber, tickerSymbol];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"index" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();
#endif
	}
	
	if ([array count] > 0) {
		return array;
	} else {
		return nil;
	}
}

/**
 Returns the fetched results controller. Creates and configures the controller if necessary.
 */
- (NSFetchedResultsController *)fetchedResultsController {
	NSAssert(self.managedObjectContext != nil, @"NSManagedObjectContext is nil");

    if (_fetchedResultsController != nil) {
        return _fetchedResultsController;
    }
    
	// Create and configure a fetch request with the Book entity.
	NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"SymbolDynamicData" inManagedObjectContext:self.managedObjectContext];
	[fetchRequest setEntity:entity];
	
	// Create the sort descriptors array.
	NSSortDescriptor *tickerDescriptor = [[NSSortDescriptor alloc] initWithKey:@"symbol.index" ascending:YES];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:tickerDescriptor, nil];
	[tickerDescriptor release];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
	self.fetchedResultsController = aFetchedResultsController;
	_fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[sortDescriptors release];
	
	return _fetchedResultsController;
}

#pragma mark -
#pragma mark Debugging methods
/*
 // Very helpful debug when things seem not to be working.
 - (BOOL)respondsToSelector:(SEL)sel {
 NSLog(@"Queried about %@", NSStringFromSelector(sel));
 return [super respondsToSelector:sel];
 }
 */

- (void)dealloc {
	[_managedObjectContext release];
	[_fetchedResultsController release];
	
	[super dealloc];
}

@end
