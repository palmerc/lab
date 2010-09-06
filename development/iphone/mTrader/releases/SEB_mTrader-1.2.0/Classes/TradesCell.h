//
//  TradesCell.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Trade;

@interface TradesCell : UITableViewCell {
@private
	Trade *_trade;
	
	UIFont *_mainFont; 
	
	UILabel *_timeLabel;
	UILabel *_priceLabel;
	UILabel *_volumeLabel;
	UILabel *_buyerLabel;
	UILabel *_sellerLabel;
}

@property (nonatomic, retain) Trade *trade;
@property (nonatomic, retain) UIFont *mainFont;
@property (nonatomic, retain) UILabel *timeLabel;
@property (nonatomic, retain) UILabel *priceLabel;
@property (nonatomic, retain) UILabel *volumeLabel;
@property (nonatomic, retain) UILabel *buyerLabel;
@property (nonatomic, retain) UILabel *sellerLabel;

@end
