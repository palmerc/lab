//
//  Symbol.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>
@class Chart;

@interface Symbol : NSObject {
	NSString *_feedNumber;
	NSString *_tickerSymbol;
	NSString *_name;
	NSString *_isin; // International Securities Identification Number
	NSNumber *type;
	NSString *orderbook;
	NSString *exchangeCode;
	
	NSNumber *lastTrade;
	NSNumber *percentChange;
	NSNumber *bidPrice;
	NSNumber *askPrice;
	NSString *askVolume;
	NSString *bidVolume;
	NSNumber *change;
	NSNumber *changeSinceLastUpdate;
	NSNumber *high;
	NSNumber *low;
	NSNumber *open;
	NSString *volume;
	Chart *chart;
}

@property (nonatomic, retain) NSString *feedTicker;
@property (nonatomic, retain) NSString *feedNumber;
@property (nonatomic, retain) NSString *tickerSymbol;
@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *isin;

@property (nonatomic, retain) NSNumber *type;
@property (nonatomic, retain) NSString *orderbook;
@property (nonatomic, retain) NSString *exchangeCode;

@property (nonatomic, retain) NSNumber *lastTrade;
@property (nonatomic, retain) NSNumber *percentChange;
@property (nonatomic, retain) NSNumber *bidPrice;
@property (nonatomic, retain) NSNumber *askPrice;
@property (nonatomic, retain) NSString *askVolume;
@property (nonatomic, retain) NSString *bidVolume;
@property (nonatomic, retain) NSNumber *change;
@property (nonatomic, retain) NSNumber *changeSinceLastUpdate;
@property (nonatomic, retain) NSNumber *high;
@property (nonatomic, retain) NSNumber *low;
@property (nonatomic, retain) NSNumber *open;
@property (nonatomic, retain) NSString *volume;
@property (nonatomic, retain) Chart *chart;
@end
