//
//  SymbolNewsView_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.08.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@interface SymbolNewsView_Phone : UIView {
	UIFont *_headlineFont;
	
	UITableView *_tableView;
	UILabel *_newsAvailableLabel;
	
	BOOL _modal;
}

@property (readonly) UIFont *headlineFont;

@property (readonly) UITableView *tableView;
@property (readonly) UILabel *newsAvailableLabel;
@property BOOL modal;

@end
