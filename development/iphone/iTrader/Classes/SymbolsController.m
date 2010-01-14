//
//  StocksController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "SymbolsController.h"
#import "iTraderCommunicator.h"
#import "Symbol.h"
#import "Feed.h"
#import "Chart.h"

@implementation SymbolsController
@synthesize updateDelegate;
@synthesize feeds = _feeds;

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
		_communicator = [iTraderCommunicator sharedManager];
		_communicator.mTraderServerDataDelegate = self;
		
		_feeds = [[NSMutableArray alloc] init];
	}
	
	return self;
}

-(void)dealloc {
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

- (void)chart:(Chart *)chart {
	NSIndexPath *symbolIndexPath = [self indexPathOfSymbol:chart.feedTicker];
	
	Feed *feed = [self.feeds objectAtIndex:symbolIndexPath.section];
	Symbol *symbol = [feed.symbols objectAtIndex:symbolIndexPath.row];
	symbol.chart = chart;
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(chartUpdated:)]) {
		[updateDelegate chartUpdated:chart];
	}
}

//18177/OSEBX;380.983;0.22;;;;;0.827
// feed/ticker
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
