//
//  OrderBookCellP.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class BidAsk;
@interface OrderBookTableCellP : UITableViewCell {
@private
	BidAsk *_bidAsk;
	
	CGFloat _maxWidth;
	CGFloat _lineHeight;
	UIFont *_mainFont;
	UILabel *_bidSizeLabel;
	UILabel *_bidValueLabel;
	UILabel *_askSizeLabel;
	UILabel *_askValueLabel;
}

@property (nonatomic, retain) BidAsk *bidAsk;
@property (nonatomic, assign) CGFloat maxWidth;
@property (nonatomic, retain) UIFont *mainFont;
@property (nonatomic, retain) UILabel *bidSizeLabel;
@property (nonatomic, retain) UILabel *bidValueLabel;
@property (nonatomic, retain) UILabel *askSizeLabel;
@property (nonatomic, retain) UILabel *askValueLabel;

- (UILabel *)createLabel;

@end
