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
	Trade *trade;
	
	UIFont *mainFont; 
	
	UILabel *time;
	UILabel *price;
	UILabel *volume;
}

@property (nonatomic, retain) Trade *trade;

- (UILabel *)createLabel;

@end
