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

- (NSString *)description {
	return [NSString stringWithFormat:@"(Symbol: %@/%@, ISIN: %@, Type: %@, Orderbook: %@, ExchangeCode: %@)", ticker, name, isin, type, orderbook, exchangeCode];
}

@end
