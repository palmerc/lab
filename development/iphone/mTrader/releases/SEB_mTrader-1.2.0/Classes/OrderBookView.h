//
//  OrderBookView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@protocol OrderBookViewDelegate;

@interface OrderBookView : UIView {
@private
	UIFont *_headerFont;
	UILabel *_askSizeLabel;
	UILabel *_askValueLabel;
	UILabel *_bidSizeLabel;
	UILabel *_bidValueLabel;
	UILabel *_orderbookAvailableLabel;
	UITableView *_tableView;
}

@property (readonly) UILabel *orderbookAvailableLabel;
@property (readonly) UITableView *tableView;

@end

