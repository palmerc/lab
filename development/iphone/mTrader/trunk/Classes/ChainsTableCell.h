//
//  ChainsTableCell.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "ChainsTableViewController.h"

@class SymbolDynamicData;

@interface ChainsTableCell : UITableViewCell {
@private
	SymbolDynamicData *_symbolDynamicData;
	
	UILabel *_tickerLabel;
	UILabel *_descriptionLabel;
	UILabel *_centerLabel;
	UILabel *_rightLabel;
	UILabel *_currencyLabel;
	UILabel *_timeLabel;
	
	CGSize tickerLabelSize;
	CenterOptions centerOption;
	RightOptions rightOption;
}

@property (nonatomic, retain) SymbolDynamicData *symbolDynamicData;
@property (nonatomic, retain) UILabel *tickerLabel;
@property (nonatomic, retain) UILabel *descriptionLabel;
@property (nonatomic, retain) UILabel *centerLabel;
@property (nonatomic, retain) UILabel *rightLabel;
@property (nonatomic, retain) UILabel *currencyLabel;
@property (nonatomic, retain) UILabel *timeLabel;
@property (assign) CenterOptions centerOption;
@property (assign) RightOptions rightOption;

@end