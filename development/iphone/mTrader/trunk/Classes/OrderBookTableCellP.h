//
//  OrderBookCellP.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;

@interface OrderBookTableCellP : UITableViewCell {
	Symbol *symbol;
	
	UILabel *bidSizeLabel;
	UILabel *bidValueLabel;
	UILabel *askSizeLabel;
	UILabel *askValueLabel;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UILabel *bidSizeLabel;
@property (nonatomic, retain) UILabel *bidValueLabel;
@property (nonatomic, retain) UILabel *askSizeLabel;
@property (nonatomic, retain) UILabel *askValueLabel;

@end
