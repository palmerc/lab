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

@implementation SymbolsController
@synthesize symbols, orderedSymbols, feeds, orderedFeeds;
@synthesize updateDelegate;

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

-(id)init {
	self = [super init];
	if (self != nil) {
		communicator = [iTraderCommunicator sharedManager];
		communicator.symbolsDelegate = self;
		symbols = [[NSMutableDictionary alloc] init];
		orderedSymbols = [[NSMutableArray alloc] init];
		feeds = [[NSMutableDictionary alloc] init];
		orderedFeeds = [[NSMutableArray alloc] init];
		// If I store the info on the phone I should load it up now.
	}
	
	return self;
}

-(void)addSymbol:(Symbol *)symbol {
	if (symbol != nil) {
		[symbol retain];
		
		// temporary assumption that ISIN is unique. Technically a MIC is also
		// required to ensure uniqueness.
		if ([symbols objectForKey:symbol.feedTicker] == nil) {
			NSUInteger index = [orderedSymbols count];
			[orderedSymbols addObject:symbol];
			[symbols setObject:[NSNumber numberWithInteger:index] forKey:symbol.feedTicker];
		}		
	}
	
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(symbolsUpdated)]) {
		[self.updateDelegate symbolsUpdated];
	}
}

-(void)addFeed:(Feed *)feed {
	// This needs to be fixed
	if (feed != nil) {
		[feed retain];
		
		if ([feeds objectForKey:feed.number] == nil) {
			NSUInteger index = [orderedFeeds count];
			[orderedFeeds addObject:feed];
			[feeds setObject:[NSNumber numberWithInteger:index] forKey:feed.number];
			
		}
	}
}

-(void)updateQuotes:(NSArray *)quotes {
	NSMutableArray *updatedQuotes = [[NSMutableArray alloc] init];
	for (NSString *quote in quotes) {
	
		NSArray *values = [quote componentsSeparatedByString:@";"];
		NSLog(@"%@", values);
		//18177/OSEBX;380.983;0.22;;;;;0.827
		// feed/ticker
		NSString *feedTicker = [values objectAtIndex:0];
		Symbol *symbol = [self.symbols objectForKey:feedTicker];
		// last trade
		if ([[values objectAtIndex:1] isEqualToString:@""] == NO) {
			symbol.lastTrade = [NSNumber numberWithFloat:[[values objectAtIndex:1] floatValue]];
		}
		// percent change
		//symbol.percentChange = [NSNumber numberWithInteger:[[values objectAtIndex:2] integerValue]];
		// bid price
		//symbol.bidPrice = [NSNumber numberWithInteger:[[values objectAtIndex:3] integerValue]];
		// ask price
		//symbol.askPrice = [NSNumber numberWithInteger:[[values objectAtIndex:4] integerValue]];
		// ask volume
		//symbol.askVolume = [NSNumber numberWithInteger:[[values objectAtIndex:5] integerValue]];
		// bid volume
		//symbol.bidVolume = [NSNumber numberWithInteger:[[values objectAtIndex:6] integerValue]];
		// change
		//symbol.change = [NSNumber numberWithInteger:[[values objectAtIndex:7] integerValue]];
		// high
		//symbol.high = [NSNumber numberWithInteger:[[values objectAtIndex:8] integerValue]];
		// low
		//symbol.low = [NSNumber numberWithInteger:[[values objectAtIndex:9] integerValue]];
		// open
		//symbol.open = [NSNumber numberWithInteger:[[values objectAtIndex:10] integerValue]];
		// volume
		//symbol.volume = [NSNumber numberWithInteger:[[values objectAtIndex:11] integerValue]];
		
		[updatedQuotes addObject:feedTicker];
	}
	
	if (updateDelegate && [updateDelegate respondsToSelector:@selector(symbolsUpdated)]) {
		[self.updateDelegate symbolsUpdated:updatedQuotes];
	}
	
	[updatedQuotes release];
}

-(void)dealloc {
	[symbols release];
	[feeds release];
	[orderedFeeds release];
	
	[super dealloc];
}
	 
	 
@end
