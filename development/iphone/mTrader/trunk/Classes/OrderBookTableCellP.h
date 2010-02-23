//
//  OrderBookCellP.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Ask;
@class Bid;
@interface OrderBookTableCellP : UITableViewCell {
@private
	Ask *ask;
	Bid *bid;
	
	UIFont *mainFont;
	UILabel *bidSizeLabel;
	UILabel *bidValueLabel;
	UILabel *askSizeLabel;
	UILabel *askValueLabel;
}

@property (nonatomic, retain) Ask *ask;
@property (nonatomic, retain) Bid *bid;
@property (nonatomic, retain) UIFont *mainFont;
@property (nonatomic, retain) UILabel *bidSizeLabel;
@property (nonatomic, retain) UILabel *bidValueLabel;
@property (nonatomic, retain) UILabel *askSizeLabel;
@property (nonatomic, retain) UILabel *askValueLabel;

- (UILabel *)createLabel;

@end
