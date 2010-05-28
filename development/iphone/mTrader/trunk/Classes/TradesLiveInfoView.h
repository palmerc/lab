//
//  TradesLiveInfoView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangle.h"

@class Symbol;

@interface TradesLiveInfoView : RoundedRectangle {
@private
	Symbol *_symbol;
	
	UILabel *_openLabel; 
	UILabel *_open;
	UILabel *_highLabel;
	UILabel *_high;
	UILabel *_lowLabel;
	UILabel *_low;
	UILabel *_vwapLabel;
	UILabel *_vwap;
	UILabel *_volumeLabel;
	UILabel *_volume;
	UILabel *_tradesLabel;
	UILabel *_trades;
	UILabel *_turnoverLabel;
	UILabel *_turnover;
	UILabel *_bLotLabel;
	UILabel *_bLot;
	UILabel *_bLotValLabel;
	UILabel *_bLotVal;
	UILabel *_avgVolLabel;
	UILabel *_avgVol;
	UILabel *_avgValLabel;
	UILabel *_avgVal;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UILabel *openLabel; 
@property (nonatomic, retain) UILabel *open;
@property (nonatomic, retain) UILabel *highLabel;
@property (nonatomic, retain) UILabel *high;
@property (nonatomic, retain) UILabel *lowLabel;
@property (nonatomic, retain) UILabel *low;
@property (nonatomic, retain) UILabel *vwapLabel;
@property (nonatomic, retain) UILabel *vwap;
@property (nonatomic, retain) UILabel *volumeLabel;
@property (nonatomic, retain) UILabel *volume;
@property (nonatomic, retain) UILabel *tradesLabel;
@property (nonatomic, retain) UILabel *trades;
@property (nonatomic, retain) UILabel *turnoverLabel;
@property (nonatomic, retain) UILabel *turnover;
@property (nonatomic, retain) UILabel *bLotLabel;
@property (nonatomic, retain) UILabel *bLot;
@property (nonatomic, retain) UILabel *bLotValLabel;
@property (nonatomic, retain) UILabel *bLotVal;
@property (nonatomic, retain) UILabel *avgVolLabel;
@property (nonatomic, retain) UILabel *avgVol;
@property (nonatomic, retain) UILabel *avgValLabel;
@property (nonatomic, retain) UILabel *avgVal;

- (void)updateSymbol;

@end
