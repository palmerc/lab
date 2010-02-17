//
//  SymbolDynamicData.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 17.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Symbol;

@interface SymbolDynamicData :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * bidSize;
@property (nonatomic, retain) NSNumber * bidVolume;
@property (nonatomic, retain) NSNumber * lastTradeChange;
@property (nonatomic, retain) NSNumber * dividend;
@property (nonatomic, retain) NSNumber * previousClose;
@property (nonatomic, retain) NSNumber * low;
@property (nonatomic, retain) NSNumber * buyLot;
@property (nonatomic, retain) NSNumber * onValue;
@property (nonatomic, retain) NSString * segment;
@property (nonatomic, retain) NSNumber * volume;
@property (nonatomic, retain) NSNumber * high;
@property (nonatomic, retain) NSNumber * VWAP;
@property (nonatomic, retain) NSNumber * openPercentChange;
@property (nonatomic, retain) NSNumber * averageValue;
@property (nonatomic, retain) NSNumber * turnover;
@property (nonatomic, retain) NSNumber * lastTradePercentChange;
@property (nonatomic, retain) NSNumber * lastTrade;
@property (nonatomic, retain) NSNumber * bidPrice;
@property (nonatomic, retain) NSDate * lastTradeTime;
@property (nonatomic, retain) NSNumber * askVolume;
@property (nonatomic, retain) NSNumber * openChange;
@property (nonatomic, retain) NSNumber * change;
@property (nonatomic, retain) NSNumber * outstandingShares;
@property (nonatomic, retain) NSNumber * changePercent;
@property (nonatomic, retain) NSNumber * buyLotValue;
@property (nonatomic, retain) NSNumber * marketCapitalization;
@property (nonatomic, retain) NSNumber * onVolume;
@property (nonatomic, retain) NSString * tradingStatus;
@property (nonatomic, retain) NSNumber * averageVolume;
@property (nonatomic, retain) NSNumber * open;
@property (nonatomic, retain) NSNumber * askSize;
@property (nonatomic, retain) NSNumber * askPrice;
@property (nonatomic, retain) NSDate * dividendDate;
@property (nonatomic, retain) Symbol * symbol;

@end



