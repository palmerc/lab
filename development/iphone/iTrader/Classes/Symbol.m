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
@synthesize lastTrade, percentChange, bidPrice, askPrice, askVolume, bidVolume, change, high, low, open, volume;

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
		high = nil;
		low = nil;
		open = nil;
		volume = nil;
	}
	
	return self;
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
	[high release];
	[low release];
	[open release];
	[volume release];
	
	[super dealloc];
}

- (NSString *)description {
	return [NSString stringWithFormat:@"(Symbol: %@/%@, ISIN: %@, Type: %@, Orderbook: %@, ExchangeCode: %@)", ticker, name, isin, type, orderbook, exchangeCode];
}

-(BOOL)isEqualToString:(NSString *)aString {
	return YES;
}

@end
