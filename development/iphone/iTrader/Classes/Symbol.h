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
	NSString *_type;
	NSString *_orderbook;
	NSString *_exchangeCode;
	NSString *_status;
	NSString *_country;
	NSString *_currency;
	NSString *_lastTrade;
	NSString *_lastTradeTime;
	NSString *_lastTradeChange;
	NSString *_lastTradePercentChange;
	NSString *_openChange;
	NSString *_openPercentChange;
	NSString *_percentChange;
	NSString *_bidPrice;
	NSString *_bidVolume;
	NSString *_bidSize; 
	NSString *_askPrice;
	NSString *_askVolume;
	NSString *_askSize;
	NSString *_onVolume;
	NSString *_onValue;
	NSString *_VWAP;
	NSString *_averageVolume;
	NSString *_averageValue;
	NSString *_marketCapitalization;
	NSString *_outstandingShares;
	NSString *_change;
	NSString *_changeSinceLastUpdate;
	NSString *_low;
	NSString *_high;
	NSString *_open;
	NSString *_previousClose;
	NSString *_volume;
	NSString *_turnover;
	NSString *_buyLot;
	NSString *_buyLotValue;
	Chart *_chart;
}

@property (assign) NSString *feedTicker;

@property (nonatomic, retain) NSString *feedNumber;
@property (nonatomic, retain) NSString *tickerSymbol;
@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *isin;
@property (nonatomic, retain) NSString *type;
@property (nonatomic, retain) NSString *orderbook;
@property (nonatomic, retain) NSString *exchangeCode;
@property (nonatomic, retain) NSString *status;
@property (nonatomic, retain) NSString *country;
@property (nonatomic, retain) NSString *currency;
@property (nonatomic, retain) NSString *lastTrade;
@property (nonatomic, retain) NSString *lastTradeTime;
@property (nonatomic, retain) NSString *lastTradeChange;
@property (nonatomic, retain) NSString *lastTradePercentChange;
@property (nonatomic, retain) NSString *openChange;
@property (nonatomic, retain) NSString *openPercentChange;
@property (nonatomic, retain) NSString *percentChange;
@property (nonatomic, retain) NSString *bidPrice;
@property (nonatomic, retain) NSString *bidVolume;
@property (nonatomic, retain) NSString *bidSize; 
@property (nonatomic, retain) NSString *askPrice;
@property (nonatomic, retain) NSString *askVolume;
@property (nonatomic, retain) NSString *askSize;
@property (nonatomic, retain) NSString *onVolume;
@property (nonatomic, retain) NSString *onValue;
@property (nonatomic, retain) NSString *VWAP;
@property (nonatomic, retain) NSString *averageVolume;
@property (nonatomic, retain) NSString *averageValue;
@property (nonatomic, retain) NSString *marketCapitalization;
@property (nonatomic, retain) NSString *outstandingShares;
@property (nonatomic, retain) NSString *change;
@property (nonatomic, retain) NSString *changeSinceLastUpdate;
@property (nonatomic, retain) NSString *low;
@property (nonatomic, retain) NSString *high;
@property (nonatomic, retain) NSString *open;
@property (nonatomic, retain) NSString *previousClose;
@property (nonatomic, retain) NSString *volume;
@property (nonatomic, retain) NSString *turnover;
@property (nonatomic, retain) NSString *buyLot;
@property (nonatomic, retain) NSString *buyLotValue;
@property (nonatomic, retain) Chart *chart;
@end
