//
//  Symbol.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Symbol.h"


@implementation Symbol
@synthesize feedTicker, feedNumber, ticker, name, isin, type, orderbook, exchangeCode;
@synthesize lastTrade, percentChange, bidPrice, askPrice, askVolume, bidVolume, change, changeSinceLastUpdate, high, low, open, volume;

- (id)init {
	self = [super init];
	if (self != nil) {
		feedTicker = nil;
		feedNumber = nil;
		ticker = nil;
		name = nil;
		isin = nil;
		type = nil;
		orderbook = nil;
		exchangeCode = nil;
		
		lastTrade = nil;
		percentChange = nil;
		bidPrice = nil;
		askPrice = nil;
		askVolume = nil;
		bidVolume = nil;
		change = nil;
		changeSinceLastUpdate = nil;
		high = nil;
		low = nil;
		open = nil;
		volume = nil;
	}
	
	return self;
}

-(void)setLastTrade:(NSNumber *)trade {
	[trade retain];	
	float lt = [lastTrade floatValue];
	float t = [trade floatValue];
	float c = t - lt;
	NSLog(@"changeSinceLastTrade: %f", c);
	self.changeSinceLastUpdate = [NSNumber numberWithFloat:c];
	[lastTrade release];
	lastTrade = nil;
	lastTrade = trade;
	
}	

- (void)dealloc {
	[feedTicker release];
	[feedNumber release];
	[ticker release];
	[name release];
	[isin release];
	[type release];
	[orderbook release];
	[exchangeCode release];	
	
	[lastTrade release];
	[percentChange release];
	[bidPrice release];
	[askPrice release];
	[askVolume release];
	[bidVolume release];
	[change release];
	[changeSinceLastUpdate release];
	[high release];
	[low release];
	[open release];
	[volume release];
	
	[super dealloc];
}

- (NSString *)description {
	return [NSString stringWithFormat:@"(Symbol: %@/%@, ISIN: %@, Type: %@, Orderbook: %@, ExchangeCode: %@)", ticker, name, isin, type, orderbook, exchangeCode];
}

@end
