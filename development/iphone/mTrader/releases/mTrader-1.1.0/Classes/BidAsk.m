// 
//  BidAsk.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "BidAsk.h"

#import "Symbol.h"

@implementation BidAsk 

@dynamic index;
@dynamic askSize;
@dynamic bidPercent;
@dynamic askPrice;
@dynamic bidPrice;
@dynamic askPercent;
@dynamic bidSize;
@dynamic symbol;

- (NSComparisonResult)compareBidSize:(BidAsk *)bidAsk {
	if ( [bidAsk.bidSize integerValue] < [self.bidSize integerValue] ) {
		return NSOrderedDescending;
	} else if ( [bidAsk.bidSize integerValue] > [self.bidSize integerValue] ) { 
		return NSOrderedAscending;
	} else {
		return NSOrderedSame;
	}
}

- (NSComparisonResult)compareAskSize:(BidAsk *)bidAsk {
	if ( [bidAsk.askSize integerValue] < [self.askSize integerValue] ) {
		return NSOrderedDescending;
	} else if ( [bidAsk.askSize integerValue] > [self.askSize integerValue] ) { 
		return NSOrderedAscending;
	} else {
		return NSOrderedSame;
	}
}

@end
