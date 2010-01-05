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
@synthesize symbols, feeds;

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
		symbols = [[NSMutableArray alloc] init];
		feeds = [[NSMutableDictionary alloc] init];
		
		// If I store the info on the phone I should load it up now.
	}
	
	return self;
}

-(void)addSymbol:(Symbol *)symbol {
	NSLog(@"Symbol: %@", symbol);
	[symbols addObject:symbol];
}

-(void)addFeed:(Feed *)feed {
	NSLog(@"Feed: %@", feed);
	[feeds setObject:feed forKey:feed.number];
}

-(void)dealloc {
	[symbols release];
	[feeds release];
	
	[super dealloc];
}
	 
	 
@end
