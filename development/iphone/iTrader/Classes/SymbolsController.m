//
//  StocksController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "SymbolsController.h"
#import "Symbol.h"
#import "Feed.h"

@implementation SymbolsController
@synthesize symbols, feeds;

-(id)init {
	self = [super init];
	if (self != nil) {
		symbols = [[NSMutableArray alloc] init];
		feeds = [[NSDictionary alloc] init];
		
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
