//
//  Symbol.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Symbol.h"


@implementation Symbol
@synthesize feedNumber, ticker, name, isin, type, orderbook, exchangeCode;

- (id)init {
	self = [super init];
	if (self != nil) {
		feedNumber = nil;
		ticker = nil;
		name = nil;
		isin = nil;
		type = nil;
		orderbook = nil;
		exchangeCode = nil;
	}
	
	return self;
}

- (void)dealloc {
	[feedNumber release];
	[ticker release];
	[name release];
	[isin release];
	[type release];
	[orderbook release];
	[exchangeCode release];	
	
	[super dealloc];
}

- (NSString *)description {
	return [NSString stringWithFormat:@"(Symbol: %@/%@, ISIN: %@, Type: %@, Orderbook: %@, ExchangeCode: %@)", ticker, name, isin, type, orderbook, exchangeCode];
}

-(BOOL)isEqualToString:(NSString *)aString {
	return YES;
}

@end
