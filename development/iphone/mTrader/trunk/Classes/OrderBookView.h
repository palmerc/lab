//
//  OrderBookView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@interface OrderBookView : UIView {
@private	
	UIButton *_orderBookButton;
	
	UIFont *_headerFont;
	UILabel *_askSizeLabel;
	UILabel *_askValueLabel;
	UILabel *_bidSizeLabel;
	UILabel *_bidValueLabel;
	UITableView *_tableView;
}

@property (nonatomic, retain) UIButton *orderBookButton;
@property (nonatomic, retain) UIFont *headerFont;
@property (nonatomic, retain) UILabel *askSizeLabel;
@property (nonatomic, retain) UILabel *askValueLabel;
@property (nonatomic, retain) UILabel *bidSizeLabel;
@property (nonatomic, retain) UILabel *bidValueLabel;
@property (nonatomic, retain) UITableView *tableView;

@end
