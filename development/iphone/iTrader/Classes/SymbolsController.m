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
		if ([symbols objectForKey:symbol.isin] == nil) {
			[symbols setObject:symbol forKey:symbol.isin];
			[orderedSymbols addObject:symbol];	
		}		
	}
}

-(void)addFeed:(Feed *)feed {
	// This needs to be fixed
	if (feed != nil) {
		[feed retain];
		
		if ([feeds objectForKey:feed.number] == nil) {
			[feeds setObject:feed forKey:feed.number];
			[orderedFeeds addObject:feed];
		}
	}
}

-(void)dealloc {
	[symbols release];
	[feeds release];
	[orderedFeeds release];
	
	[super dealloc];
}
	 
	 
@end
