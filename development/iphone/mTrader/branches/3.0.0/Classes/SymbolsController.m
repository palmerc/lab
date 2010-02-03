//
//  StocksController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "SymbolsController.h"
#import "mTraderCommunicator.h"
#import "Symbol.h"
#import "Feed.h"
#import "Chart.h"

@implementation SymbolsController
@synthesize updateDelegate;
@synthesize communicator = _communicator;
@synthesize feeds = _feeds;
@synthesize exchanges = _exchanges;

/**
 * Singleton Setup
 *
 */
static SymbolsController *sharedSymbolsController = nil;

+ (SymbolsController *)sharedManager {
	if (sharedSymbolsController == nil) {
		sharedSymbolsController = [[super allocWithZone:NULL] init];
	}
	return sharedSymbolsController;
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

/**
 * Typical Object Handling
 *
 */

-(id)init {
	self = [super init];
	if (self != nil) {
		_communicator = [mTraderCommunicator sharedManager];
		_communicator.mTraderServerDataDelegate = self;
		
		_feeds = [[NSMutableArray alloc] init];
		_exchanges = [[NSMutableArray alloc] init];
	}
	
	return self;
}

-(void)dealloc {
	[self.exchanges release];
	[self.feeds release];
	
	[super dealloc];
}

/**
 * The worker bees
 *
 */

-(void)addSymbol:(Symbol *)symbol {
	NSInteger sectionIndex = [self indexOfFeedWithFeedNumber:symbol.feedNumber];
	Feed *feed = [self.feeds objectAtIndex:sectionIndex];
	
	if ([feed.symbols indexOfObject:symbol] == NSNotFound) {
		[feed.symbols addObject:symbol];
		
		if (updateDelegate && [updateDelegate respondsToSelector:@selector(symbolsAdded:)]) {
			[self.updateDelegate symbolsAdded:[NSArray arrayWithObject:symbol]];
		}
	}
}

- (void)addFeed:(Feed *)feed {
	NSInteger sectionIndex = [self.feeds indexOfObject:feed];
	if (sectionIndex == NSNotFound) { // We haven't seen it before
		[self.feeds addObject:feed];
		
		if (updateDelegate && [updateDelegate respondsToSelector:@selector(feedAdded:)]) {
			[self.updateDelegate feedAdded:feed];
		}
	}
}

- (void)addSymbol:(Symbol *)symbol withFeed:(Feed *)feed {
	[self addFeed:feed];
	[self addSymbol:symbol];
}

-(void) removeSymbol:(NSIndexPath *)indexPath {
	Feed *feed = [self.feeds objectAtIndex:indexPath.section];
	Symbol *symbol = [self symbolAtIndexPath:indexPath];
	[self.communicator removeSecurity:symbol.feedTicker];

	[feed.symbols removeObjectAtIndex:indexPath.row];
	
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(symbolRemoved:)]) {
		[self.updateDelegate symbolRemoved:indexPath];
	}
}

- (NSInteger)indexOfFeed:(Feed *)feed {
	return [self.feeds indexOfObject:feed];
}

- (NSInteger)indexOfFeedWithFeedNumber:(NSString *)feedNumber {
	Feed *feed;
	for (Feed *aFeed in self.feeds) {
		if ([aFeed.feedNumber isEqual:feedNumber]) {
			feed = aFeed;
			break;
		}
	}
	
	return [self.feeds indexOfObject:feed];
}

- (NSIndexPath *)indexPathOfSymbol:(NSString *)feedTicker {
	NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
	NSString *feedNumber = [feedTickerComponents objectAtIndex:0];
	
	NSInteger sectionIndex = [self indexOfFeedWithFeedNumber:feedNumber];
	if (sectionIndex != NSNotFound) {
		Feed *feed = [self.feeds objectAtIndex:sectionIndex];
		Symbol *symbol;
		for (Symbol *aSymbol in feed.symbols) {
			if ([aSymbol.feedTicker isEqual:feedTicker]) {
				symbol = aSymbol;
				break;
			}
		}
		
		NSInteger rowIndex = [feed.symbols indexOfObject:symbol];
		
		if (rowIndex != NSNotFound) {
			return [NSIndexPath indexPathForRow:rowIndex inSection:sectionIndex];
		}
	}
	return nil;
}

- (Symbol *)symbolAtIndexPath:(NSIndexPath *)indexPath {
	Feed *feed = [self.feeds objectAtIndex:indexPath.section];
	return [feed.symbols objectAtIndex:indexPath.row];
}

- (Symbol *)symbolWithFeedTicker:(NSString *)feedTicker {
	NSIndexPath *indexPath = [self indexPathOfSymbol:feedTicker];
	return [self symbolAtIndexPath:indexPath];
}

- (void)chart:(Chart *)chart {
	NSIndexPath *symbolIndexPath = [self indexPathOfSymbol:chart.feedTicker];
	
	Feed *feed = [self.feeds objectAtIndex:symbolIndexPath.section];
	Symbol *symbol = [feed.symbols objectAtIndex:symbolIndexPath.row];
	symbol.chart = chart;
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(staticUpdated:)]) {
		[updateDelegate staticUpdated:symbol.feedTicker];
	}
}

-(void)addExchanges:(NSArray *)exchanges {
	for (NSString *exchange in exchanges) {
		NSRange exchangeRange = [exchange rangeOfString:@"]"];
		exchange = [exchange substringToIndex:exchangeRange.location + 1];
		[self.exchanges addObject:exchange];
	}
}

-(void)updateQuotes:(NSArray *)quotes {	
	NSMutableArray *updatedQuotes = [[NSMutableArray alloc] init];
	
	for (NSString *quote in quotes) {
		NSArray *values = [self cleanQuote:quote];
		
		NSString *feedTicker = [values objectAtIndex:0];
		NSIndexPath *symbolIndexPath = [self indexPathOfSymbol:feedTicker];
		
		Feed *feed = [self.feeds objectAtIndex:symbolIndexPath.section];
		Symbol *symbol = [feed.symbols objectAtIndex:symbolIndexPath.row];
		
		// last trade
		NSString *lastTrade = [values objectAtIndex:1];
		if ([lastTrade isEqualToString:@""] == NO) {
			symbol.lastTrade = lastTrade;
		}
		
		// percent change
		NSString *percentChange = [values objectAtIndex:2];
		if ([percentChange isEqualToString:@""] == NO) {
			symbol.percentChange = percentChange;
		}
		
		// bid price
		NSString *bidPrice = [values objectAtIndex:3];
		if ([bidPrice isEqualToString:@""] == NO) {
			symbol.bidPrice = bidPrice;
		}
		
		// ask price
		NSString *askPrice = [values objectAtIndex:4];
		if ([askPrice isEqualToString:@""] == NO) {
			symbol.askPrice = askPrice;
		}
		
		// ask volume
		NSString *askVolume = [values objectAtIndex:5];
		if ([askVolume isEqualToString:@""] == NO) {
			symbol.askVolume = askVolume;
		}
		
		// bid volume
		NSString *bidVolume = [values objectAtIndex:6];
		if ([bidVolume isEqualToString:@""] == NO) {
			symbol.bidVolume = bidVolume;
		}
		
		// change
		NSString *change = [values objectAtIndex:7];
		if ([change isEqualToString:@""] == NO) {
			symbol.change = change;
		}
		
		// high
		NSString *high = [values objectAtIndex:8];
		if ([high isEqualToString:@""] == NO) {
			symbol.high = high;
		}
		
		// low
		NSString *low = [values objectAtIndex:9];
		if ([low isEqualToString:@""] == NO) {
			symbol.low = low;
		}
		
		// open
		NSString *open = [values objectAtIndex:10];
		if ([open isEqualToString:@""] == NO) {
			symbol.open = open;
		}
		
		// volume
		NSString *volume = [values objectAtIndex:11];
		if ([volume isEqualToString:@""] == NO) {
			symbol.volume = volume;
		}
		
		[updatedQuotes addObject:feedTicker];
	}
	
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(symbolsUpdated:)]) {
		[self.updateDelegate symbolsUpdated:updatedQuotes];
	}
	
	[updatedQuotes release];
}

- (void)staticUpdates:(NSDictionary *)updateDictionary {
	Symbol *symbol = [self symbolWithFeedTicker:[updateDictionary objectForKey:@"feedTicker"]];
	
	
	if ([updateDictionary objectForKey:@"Bid"]) {
		symbol.bidPrice = [updateDictionary objectForKey:@"Bid"];
	}
	if ([updateDictionary objectForKey:@"B Size"]) {
		symbol.bidSize = [updateDictionary objectForKey:@"B Size"];
	}
	if ([updateDictionary objectForKey:@"Ask"]) { 
		symbol.askPrice = [updateDictionary objectForKey:@"Ask"];
	}
	if ([updateDictionary objectForKey:@"A Size"]) { 
		symbol.askSize = [updateDictionary objectForKey:@"A Size"];
	}
	if ([updateDictionary objectForKey:@"Pr Cls"]) { 
		symbol.previousClose = [updateDictionary objectForKey:@"Pr Cls"];
	}
	if ([updateDictionary objectForKey:@"Open"]) { 
		symbol.open = [updateDictionary objectForKey:@"Open"];
	}
	if ([updateDictionary objectForKey:@"High"]) { 
		symbol.high = [updateDictionary objectForKey:@"High"];
	}
	if ([updateDictionary objectForKey:@"Low"]) { 
		symbol.low = [updateDictionary objectForKey:@"Low"];
	}
	if ([updateDictionary objectForKey:@"Last"]) { 
		symbol.lastTrade = [updateDictionary objectForKey:@"Last"];
	}
	if ([updateDictionary objectForKey:@"L +/-"]) { 
		symbol.lastTradeChange = [updateDictionary objectForKey:@"L +/-"];
	}
	if ([updateDictionary objectForKey:@"L +/-%"]) {
		symbol.lastTradePercentChange = [updateDictionary objectForKey:@"L +/-%"];
	}
	if ([updateDictionary objectForKey:@"O +/-"]) { 
		symbol.openChange = [updateDictionary objectForKey:@"O +/-"];
	}
	if ([updateDictionary objectForKey:@"O +/-%"]) {
		symbol.openPercentChange = [updateDictionary objectForKey:@"O +/-%"];
	}
	if ([updateDictionary objectForKey:@"Volume"]) { 
		symbol.volume = [updateDictionary objectForKey:@"Volume"];
	}
	if ([updateDictionary objectForKey:@"Turnover"]) {
		symbol.turnover = [updateDictionary objectForKey:@"Turnover"];
	}
	if ([updateDictionary objectForKey:@"OnVolume"]) {
		symbol.onVolume = [updateDictionary objectForKey:@"OnVolume"];
	}
	if ([updateDictionary objectForKey:@"OnValue"]) { 
		symbol.onValue = [updateDictionary objectForKey:@"OnValue"];
	}
	if ([updateDictionary objectForKey:@"Time"]) { 
		symbol.lastTradeTime = [updateDictionary objectForKey:@"Time"];
	}
	if ([updateDictionary objectForKey:@"VWAP"]) { 
		symbol.VWAP = [updateDictionary objectForKey:@"VWAP"];
	}
	if ([updateDictionary objectForKey:@"AvgVol"]) {
		symbol.averageVolume = [updateDictionary objectForKey:@"AvgVol"];
	}
	if ([updateDictionary objectForKey:@"AvgVal"]) { 
		symbol.averageValue = [updateDictionary objectForKey:@"AvgVal"];
	}
	if ([updateDictionary objectForKey:@"Status"]) { 
		symbol.status = [updateDictionary objectForKey:@"Status"];
	}
	if ([updateDictionary objectForKey:@"B Lot"]) { 
		symbol.buyLot = [updateDictionary objectForKey:@"B Lot"];
	}
	if ([updateDictionary objectForKey:@"BLValue"]) { 
		symbol.buyLotValue = [updateDictionary objectForKey:@"BLValue"];
	}
	if ([updateDictionary objectForKey:@"Shares"]) {
		symbol.outstandingShares = [updateDictionary objectForKey:@"Shares"];
	}
	if ([updateDictionary objectForKey:@"M Cap"]) { 
		symbol.marketCapitalization = [updateDictionary objectForKey:@"M Cap"];
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
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(staticUpdated:)]) {
		[updateDelegate staticUpdated:symbol.feedTicker];
	}
}

-(NSArray *)cleanQuote:(NSString *)quote {
	NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
	NSArray *providedValues = [quote componentsSeparatedByString:@";"];
	
	NSMutableArray *paddedArray = [[NSMutableArray alloc] init];
	for (NSString *value in providedValues) {
		[paddedArray addObject:[value stringByTrimmingCharactersInSet:whitespaceAndNewline]];
	}
		
	for (int i = [paddedArray count] - 1; i < 12; i++) {
		[paddedArray addObject:@""];
	}
	
	NSArray *finalProduct = [NSArray arrayWithArray:paddedArray];
	[paddedArray release];
	
	return finalProduct;
}

	
-(NSArray *)cleanStrings:(NSArray *)strings {
	NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
	NSMutableArray *mutableProduct = [[NSMutableArray alloc] init];
	
	for (NSString *string in strings) {
		[mutableProduct addObject:[string stringByTrimmingCharactersInSet:whitespaceAndNewline]];
	}
	
	NSArray *finalProduct = [NSArray arrayWithArray:mutableProduct];
	[mutableProduct release];
	
	return finalProduct;
}

@end
