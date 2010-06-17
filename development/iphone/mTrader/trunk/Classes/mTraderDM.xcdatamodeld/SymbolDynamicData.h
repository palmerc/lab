//
//  SymbolDynamicData.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.06.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Symbol;

@interface SymbolDynamicData :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * bidSize;
@property (nonatomic, retain) NSString * bidVolume;
@property (nonatomic, retain) NSNumber * dividend;
@property (nonatomic, retain) NSNumber * previousClose;
@property (nonatomic, retain) NSNumber * low;
@property (nonatomic, retain) NSString * buyLot;
@property (nonatomic, retain) NSString * onValue;
@property (nonatomic, retain) NSString * segment;
@property (nonatomic, retain) NSString * volume;
@property (nonatomic, retain) NSNumber * high;
@property (nonatomic, retain) NSNumber * VWAP;
@property (nonatomic, retain) NSNumber * openPercentChange;
@property (nonatomic, retain) NSString * averageValue;
@property (nonatomic, retain) NSString * turnover;
@property (nonatomic, retain) NSString * trades;
@property (nonatomic, retain) NSNumber * lastTrade;
@property (nonatomic, retain) NSNumber * bidPrice;
@property (nonatomic, retain) NSDate * lastTradeTime;
@property (nonatomic, retain) NSString * askVolume;
@property (nonatomic, retain) NSNumber * openChange;
@property (nonatomic, retain) NSNumber * change;
@property (nonatomic, retain) NSString * outstandingShares;
@property (nonatomic, retain) NSNumber * changePercent;
@property (nonatomic, retain) NSString * buyLotValue;
@property (nonatomic, retain) NSString * marketCapitalization;
@property (nonatomic, retain) NSString * onVolume;
@property (nonatomic, retain) NSString * tradingStatus;
@property (nonatomic, retain) NSString * averageVolume;
@property (nonatomic, retain) NSNumber * changeArrow;
@property (nonatomic, retain) NSNumber * open;
@property (nonatomic, retain) NSString * askSize;
@property (nonatomic, retain) NSNumber * changeFlash;
@property (nonatomic, retain) NSNumber * askPrice;
@property (nonatomic, retain) NSString * dividendDate;
@property (nonatomic, retain) Symbol * symbol;

@end



