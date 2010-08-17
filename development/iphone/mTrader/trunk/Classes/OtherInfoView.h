//
//  OtherInfoView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangleFrame.h"

@class Symbol;

@interface OtherInfoView : RoundedRectangleFrame {
@private
	Symbol *_symbol;
	
	UIViewController *_viewController;
	UILabel *_description; 
	UILabel *_isinLabel;
	UILabel *_isin;
	UILabel *_segmentLabel;
	UILabel *_segment;
	UILabel *_currencyLabel;
	UILabel *_currency;
	UILabel *_countryLabel;
	UILabel *_country;
	UILabel *_exchangeLabel;
	UILabel *_exchange;
	UILabel *_statusLabel;
	UILabel *_status;
	UILabel *_dividendLabel;
	UILabel *_dividend;
	UILabel *_dividendDateLabel;
	UILabel *_dividendDate;
	UILabel *_sharesLabel;
	UILabel *_shares;
	UILabel *_marketCapLabel;
	UILabel *_marketCap;
	UILabel *_previousCloseLabel;
	UILabel *_previousClose;
	UILabel *_onVolumeLabel;
	UILabel *_onVolume;
	UILabel *_onValueLabel;
	UILabel *_onValue;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UIViewController *viewController;
@property (nonatomic, retain) UILabel *description; 
@property (nonatomic, retain) UILabel *isinLabel;
@property (nonatomic, retain) UILabel *isin;
@property (nonatomic, retain) UILabel *segmentLabel;
@property (nonatomic, retain) UILabel *segment;
@property (nonatomic, retain) UILabel *currencyLabel;
@property (nonatomic, retain) UILabel *currency;
@property (nonatomic, retain) UILabel *countryLabel;
@property (nonatomic, retain) UILabel *country;
@property (nonatomic, retain) UILabel *exchangeLabel;
@property (nonatomic, retain) UILabel *exchange;
@property (nonatomic, retain) UILabel *statusLabel;
@property (nonatomic, retain) UILabel *status;
@property (nonatomic, retain) UILabel *dividendLabel;
@property (nonatomic, retain) UILabel *dividend;
@property (nonatomic, retain) UILabel *dividendDateLabel;
@property (nonatomic, retain) UILabel *dividendDate;
@property (nonatomic, retain) UILabel *sharesLabel;
@property (nonatomic, retain) UILabel *shares;
@property (nonatomic, retain) UILabel *marketCapLabel;
@property (nonatomic, retain) UILabel *marketCap;
@property (nonatomic, retain) UILabel *previousCloseLabel;
@property (nonatomic, retain) UILabel *previousClose;
@property (nonatomic, retain) UILabel *onVolumeLabel;
@property (nonatomic, retain) UILabel *onVolume;
@property (nonatomic, retain) UILabel *onValueLabel;
@property (nonatomic, retain) UILabel *onValue;

- (void)updateSymbol;

@end
