//
//  SymbolDataController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "SymbolDataController.h"

#import "mTraderCommunicator.h"
#import "QFields.h"
#import "StringHelpers.h"

#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Chart.h"
#import "NewsFeed.h"

static SymbolDataController *sharedDataController = nil;

@implementation SymbolDataController
@synthesize communicator;
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize managedObjectContext = _managedObjectContext;

- (id)init {
	self = [super init];
	if (self != nil) {
		communicator = [mTraderCommunicator sharedManager];
		communicator.symbolsDelegate = self;
		
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
+ (SymbolDataController *)sharedManager {
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
		abort(); // Fail
	}
}


#pragma mark -
#pragma mark Delegation
/**
 * Delegation
 */

- (void)addNewsFeeds:(NSArray *)feeds {
	feeds = [StringHelpers cleanComponents:feeds];
	for (NSString *feed in feeds) {
		
		// Separate the Description from the mCode
		NSRange leftBracketRange = [feed rangeOfString:@"["];
		NSRange rightBracketRange = [feed rangeOfString:@"]"];
		NSRange leftParenthesisRange = [feed rangeOfString:@"("];
		NSRange rightParenthesisRange = [feed rangeOfString:@")"];
		
		NSRange typeRange;
		typeRange.location = leftParenthesisRange.location + 1;
		typeRange.length = rightParenthesisRange.location - typeRange.location;
		NSString *typeCode = [feed substringWithRange:typeRange]; // (S) 
		
		NSRange mCodeRange;
		mCodeRange.location = leftBracketRange.location + 1;
		mCodeRange.length = rightBracketRange.location - mCodeRange.location;
		NSString *mCode = [feed substringWithRange:mCodeRange]; // OSS
		
		NSRange descriptionRange;
		descriptionRange.location = 0;
		descriptionRange.length = leftBracketRange.location - 1;
		NSString *feedName = [feed substringWithRange:descriptionRange]; // Oslo Stocks
		
		NewsFeed *newsFeed = [self fetchNewsFeed:mCode];
		if (newsFeed == nil) {
			newsFeed = (NewsFeed *)[NSEntityDescription insertNewObjectForEntityForName:@"NewsFeed" inManagedObjectContext:self.managedObjectContext];
		}
		
		newsFeed.mCode = mCode;
		newsFeed.name = feedName;
		newsFeed.type = typeCode;
	}
	
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		// Handle the error
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();
	}
}

- (void)addExchanges:(NSArray *)exchanges {
	exchanges = [StringHelpers cleanComponents:exchanges];
	for (NSString *exchangeCode in exchanges) {
		
		// Separate the Description from the mCode
		NSRange leftBracketRange = [exchangeCode rangeOfString:@"["];
		NSRange rightBracketRange = [exchangeCode rangeOfString:@"]"];
		NSRange leftParenthesisRange = [exchangeCode rangeOfString:@"("];
		NSRange rightParenthesisRange = [exchangeCode rangeOfString:@")"];
		
		NSRange typeRange;
		typeRange.location = leftParenthesisRange.location + 1;
		typeRange.length = rightParenthesisRange.location - typeRange.location;
		NSString *typeCode = [exchangeCode substringWithRange:typeRange]; // (S) 
		
		NSRange mCodeRange;
		mCodeRange.location = leftBracketRange.location + 1;
		mCodeRange.length = rightBracketRange.location - mCodeRange.location;
		NSString *mCode = [exchangeCode substringWithRange:mCodeRange]; // OSS
		
		NSRange descriptionRange;
		descriptionRange.location = 0;
		descriptionRange.length = leftBracketRange.location - 1;
		NSString *feedName = [exchangeCode substringWithRange:descriptionRange]; // Oslo Stocks
		
		Feed *feed = [self fetchFeedByName:feedName];
		if (feed == nil) {
			feed = (Feed *)[NSEntityDescription insertNewObjectForEntityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
		}
		
		feed.mCode = mCode;
		feed.feedName = feedName;
		feed.typeCode = typeCode;
	}
	
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		// Handle the error
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();
	}
}

- (void)replaceAllSymbols:(NSString *)symbols {
	[self deleteAllSymbols];
	[self addSymbols:symbols];
}

- (void)addSymbols:(NSString *)symbols {
	// Core Data Setup - This not only grabs the existing results but also setups up the FetchController

	static NSInteger FEED_TICKER = 0;
	static NSInteger TICKER_SYMBOL = 1;
	static NSInteger COMPANY_NAME = 2;
	static NSInteger EXCHANGE_CODE = 3;
	static NSInteger TYPE = 4;
	static NSInteger ORDER_BOOK = 5;
	static NSInteger ISIN = 6;
	
	// insert the objects
	NSArray *rows = [symbols componentsSeparatedByString:@":"];	
	for (NSString *row in rows) {
		NSArray *stockComponents = [row componentsSeparatedByString:@";"];
		stockComponents = [StringHelpers cleanComponents:stockComponents];
		NSString *feedTicker = [stockComponents objectAtIndex:FEED_TICKER];
		NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
		NSString *feedNumberString = [feedTickerComponents objectAtIndex:0];
		NSNumber *feedNumber = [NSNumber numberWithInteger:[feedNumberString integerValue]];
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
		
		NSRange descriptionRange;
		descriptionRange.location = 0;
		descriptionRange.length = leftBracketRange.location - 1;
		NSString *feedName = [exchangeCode substringWithRange:descriptionRange]; // Oslo Stocks
		
		// Prevent double insertions
		Feed *feed = [self fetchFeed:mCode];
		if (feed == nil) {
			feed = (Feed *)[NSEntityDescription insertNewObjectForEntityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
			feed.mCode = mCode; // OSS
			feed.feedName = feedName;	// Oslo Stocks
			feed.typeCode = @"";
		}
		
		feed.feedNumber = feedNumber; // 18177
		
		Symbol *symbol = [self fetchSymbol:tickerSymbol withFeed:mCode]; 
		if (symbol == nil) {
			symbol = (Symbol *)[NSEntityDescription insertNewObjectForEntityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
			symbol.tickerSymbol = tickerSymbol;
			
			symbol.index = [NSNumber numberWithInteger:[[[self.fetchedResultsController sections] objectAtIndex:0] numberOfObjects]];
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
		}
	}
	// save the objects
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		// Handle the error
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
}

/**
 * This method should receive a list of symbols that have been updated and should
 * update any rows necessary.
 */
- (void)updateSymbols:(NSArray *)updates {
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
				symbol.symbolDynamicData.lastTradeTime = [NSNumber numberWithInteger:[timeStamp integerValue]];
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
				symbol.symbolDynamicData.changeArrow = [NSNumber numberWithInteger:[changeArrow integerValue]];
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
			
			NSFetchRequest *bidRequest = [[[NSFetchRequest alloc] init] autorelease];
			NSEntityDescription *bidEntity = [NSEntityDescription entityForName:@"Bid" inManagedObjectContext:self.managedObjectContext];
			[bidRequest setEntity:bidEntity];
			
			NSPredicate *bidPredicate = [NSPredicate predicateWithFormat:@"(symbol.feed.feedNumber=%@) AND (symbol.tickerSymbol=%@)", feedNumber, tickerSymbol];
			[bidRequest setPredicate:bidPredicate];
			
			NSSortDescriptor *bidSortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"index" ascending:YES];
			[bidRequest setSortDescriptors:[NSArray arrayWithObject:bidSortDescriptor]];
			[bidSortDescriptor release];
			
			NSError *bidError = nil;
			NSArray *bidsArray = [self.managedObjectContext executeFetchRequest:bidRequest error:&bidError];
			if (bidsArray == nil)
			{
				NSLog(@"Unresolved error %@, %@", bidError, [bidError userInfo]);
			}
			
			NSFetchRequest *askRequest = [[[NSFetchRequest alloc] init] autorelease];
			NSEntityDescription *askEntity = [NSEntityDescription entityForName:@"Ask" inManagedObjectContext:self.managedObjectContext];
			[askRequest setEntity:askEntity];
			
			NSPredicate *askPredicate = [NSPredicate predicateWithFormat:@"(symbol.feed.feedNumber=%@) AND (symbol.tickerSymbol=%@)", feedNumber, tickerSymbol];
			[askRequest setPredicate:askPredicate];
			
			NSSortDescriptor *askSortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"index" ascending:YES];
			[askRequest setSortDescriptors:[NSArray arrayWithObject:askSortDescriptor]];
			[askSortDescriptor release];
			
			NSError *askError = nil;
			NSArray *asksArray = [self.managedObjectContext executeFetchRequest:askRequest error:&askError];
			if (asksArray == nil)
			{
				NSLog(@"Unresolved error %@, %@", askError, [askError userInfo]);
			}

			// Bid then Ask - 

			//NSArray *orderBook = [[update valueForKey:orderBookKey] componentsSeparatedByString:@"#"];
//			orderBook = [StringHelpers cleanComponents:orderBook];
//			NSInteger half = [orderBook count] / 2;
//			
//			for (int i = 0; i < half; i++) {
//				NSString *orderBookString = [orderBook objectAtIndex:i];
//				if (![orderBookString isEqualToString:@""]) {
//					[NSEntityDescription insertNewObjectForEntityForName:@"Bid" inManagedObjectContext:self.managedObjectContext];
//					
//					NSArray *pieces = [orderBookString componentsSeparatedByString:@"\\"];
//					[bidsArray objectAtIndex:i];
//					NSString *value = [pieces objectAtIndex:0];
//					NSString *size = [pieces objectAtIndex:1];
//					
//					float multiplier = 1.0;
//					if ([size rangeOfString:@"k"].location != NSNotFound) {
//						multiplier = 1000.0;
//					} else if ([size rangeOfString:@"m"].location != NSNotFound) {
//						multiplier = 1000000.0;
//					}
//					
//					bid.bidValue = [NSNumber numberWithDouble:[value doubleValue]];
//					bid.bidSize = [NSNumber numberWithInteger:[size integerValue] * multiplier];
//					[bids insertObject:bid atIndex:i];
//					[bid release];
//				}
//			}
//			
//			for (int i = 0; i < half; i++) {
//				NSString *orderBookString = [orderBook objectAtIndex:i + half];
//				if (![orderBookString isEqualToString:@""] && ![orderBookString isEqualToString:@"/"]) {
//					if ([asks count] > i) {
//						[asks removeObjectAtIndex:i];
//					}
//					
//					if ([orderBookString rangeOfString:@"/"].location == 0) {
//						orderBookString = [orderBookString substringFromIndex:1];
//					}
//					NSArray *pieces = [orderBookString componentsSeparatedByString:@"\\"];
//					[NSEntityDescription insertNewObjectForEntityForName:@"Ask" inManagedObjectContext:self.managedObjectContext];
//					NSString *value = [pieces objectAtIndex:0];
//					NSString *size = [pieces objectAtIndex:1];
//					
//					float multiplier = 1.0;
//					if ([size rangeOfString:@"k"].location != NSNotFound) {
//						multiplier = 1000.0;
//					} else if ([size rangeOfString:@"m"].location != NSNotFound) {
//						multiplier = 1000000.0;
//					}
//					
//					ask.askValue = [NSNumber numberWithDouble:[value doubleValue]];
//					ask.askSize = [NSNumber numberWithInteger:[size integerValue] * multiplier];
//					[asks insertObject:ask atIndex:i];
//					[ask release];
//				}
//			}
			
			asksArray = nil;
			bidsArray = nil;
		}
	}
		
	NSError *saveError;
	if (![self.managedObjectContext save:&saveError]) {
		NSLog(@"Unresolved error %@, %@", saveError, [saveError userInfo]);
	}
}

- (NewsFeed *)fetchNewsFeed:(NSString *)mCode {
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
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}


- (Feed *)fetchFeed:(NSString *)mCode {
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
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (Feed *)fetchFeedByName:(NSString *)feedName {
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
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}	
}

- (void)deleteAllSymbols {
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	[request setIncludesPropertyValues:NO];
	[request setIncludesSubentities:NO];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil) {
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
	}
	
	for (Symbol *symbol in array) {
		[self.managedObjectContext deleteObject:symbol];
	}
}

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber {
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
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}


- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode {
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
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (void)removeSymbol:(Symbol *)symbol {
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", symbol.feed.feedNumber, symbol.tickerSymbol];
	[communicator removeSecurity:feedTicker];
	
	[self.managedObjectContext deleteObject:symbol];
}

/**
 Returns the fetched results controller. Creates and configures the controller if necessary.
 */
- (NSFetchedResultsController *)fetchedResultsController {
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
