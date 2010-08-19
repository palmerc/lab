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

@end
