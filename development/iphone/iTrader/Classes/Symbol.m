//
//  Symbol.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Symbol.h"


@implementation Symbol
@synthesize feedNumber = _feedNumber;
@synthesize tickerSymbol = _tickerSymbol; 
@synthesize name = _name;
@synthesize isin = _isin;
@synthesize type, orderbook, exchangeCode;
@synthesize lastTrade, percentChange, bidPrice, askPrice, askVolume, bidVolume, change, changeSinceLastUpdate, high, low, open, volume;

- (id)init {
	self = [super init];
	if (self != nil) {
		_feedNumber = nil;
		_tickerSymbol = nil;
		_name = nil;
		_isin = nil;
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

- (NSString *)feedTicker {
	return [NSString stringWithFormat:@"%@/%@", self.feedNumber, self.tickerSymbol];
}

- (void)setFeedTicker:(NSString *)feedTicker {
	NSArray *components = [feedTicker componentsSeparatedByString:@"/"];
	assert([components count] == 2);
	
	// Format of feedTicker string is feedNumber/tickerSymbol
	NSString *feedNumber = [components objectAtIndex:0];
	NSString *tickerSymbol = [components objectAtIndex:1];
	
	self.feedNumber = feedNumber;
	self.tickerSymbol = tickerSymbol;
}

-(void)setLastTrade:(NSNumber *)trade {
	[trade retain];	
	float lt = [lastTrade floatValue];
	float t = [trade floatValue];
	float c = t - lt;
	//NSLog(@"changeSinceLastTrade: %f", c);
	self.changeSinceLastUpdate = [NSNumber numberWithFloat:c];
	[lastTrade release];
	lastTrade = nil;
	lastTrade = trade;
	
}	

- (void)dealloc {
	[self.feedNumber release];
	[self.tickerSymbol release];
	[self.name release];
	[self.isin release];
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
	return [NSString stringWithFormat:@"(Symbol: %@ %@, ISIN: %@, Type: %@, Orderbook: %@, ExchangeCode: %@)", self.feedTicker, self.name, self.isin, type, orderbook, exchangeCode];
}

@end
