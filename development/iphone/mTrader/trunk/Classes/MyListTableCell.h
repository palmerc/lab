//
//  MyListTableCell.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "MyStocksViewController.h"

@class SymbolDynamicData;

@interface MyListTableCell : UITableViewCell {
	SymbolDynamicData *symbolDynamicData;
	
	UILabel *tickerLabel;
	UILabel *descriptionLabel;
	UIButton *centerButton;
	UIButton *rightButton;
	UILabel *timeLabel;
	
@private
	CGSize tickerLabelSize;
	CenterButtonOptions centerButtonOption;
	RightButtonOptions rightButtonOption;
}

@property (nonatomic, retain) SymbolDynamicData *symbolDynamicData;
@property (nonatomic, retain) UILabel *tickerLabel;
@property (nonatomic, retain) UILabel *descriptionLabel;
@property (nonatomic, retain) UIButton *centerButton;
@property (nonatomic, retain) UIButton *rightButton;
@property (nonatomic, retain) UILabel *timeLabel;
@property (assign) CenterButtonOptions centerButtonOption;
@property (assign) RightButtonOptions rightButtonOption;

@end