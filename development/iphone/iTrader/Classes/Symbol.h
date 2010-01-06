//
//  Symbol.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Symbol : NSObject {
	NSString *feedTicker; // This is how a stock is uniquely identified in mTrader
	NSString *feedNumber;
	NSString *ticker;
	NSString *name;
	NSString *isin; // International Securities Identification Number
	NSNumber *type;
	NSString *orderbook;
	NSNumber *exchangeCode;
	
	NSNumber *lastTrade;
	NSNumber *percentChange;
	NSNumber *bidPrice;
	NSNumber *askPrice;
	NSNumber *askVolume;
	NSNumber *bidVolume;
	NSNumber *change;
	NSNumber *changeSinceLastUpdate;
	NSNumber *high;
	NSNumber *low;
	NSNumber *open;
	NSNumber *volume;
}

@property (nonatomic, retain) NSString *feedTicker;
@property (nonatomic, retain) NSString *feedNumber;
@property (nonatomic, retain) NSString *ticker;
@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *isin;
@property (nonatomic, retain) NSNumber *type;
@property (nonatomic, retain) NSString *orderbook;
@property (nonatomic, retain) NSNumber *exchangeCode;

@property (nonatomic, retain) NSNumber *lastTrade;
@property (nonatomic, retain) NSNumber *percentChange;
@property (nonatomic, retain) NSNumber *bidPrice;
@property (nonatomic, retain) NSNumber *askPrice;
@property (nonatomic, retain) NSNumber *askVolume;
@property (nonatomic, retain) NSNumber *bidVolume;
@property (nonatomic, retain) NSNumber *change;
@property (nonatomic, retain) NSNumber *changeSinceLastUpdate;
@property (nonatomic, retain) NSNumber *high;
@property (nonatomic, retain) NSNumber *low;
@property (nonatomic, retain) NSNumber *open;
@property (nonatomic, retain) NSNumber *volume;

@end
