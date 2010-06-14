//
//  MyListTableCellPad.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@class SymbolDynamicData;

@interface MyListTableCellPad : UITableViewCell {
@private
	SymbolDynamicData *_symbolDynamicData;
	
	UILabel *_tickerLabel;
	UILabel *_descriptionLabel;
	
	UILabel *_bidLabel;
	UILabel *_askLabel;
	UILabel *_lastLabel;
	UILabel *_lastChangeLabel;
	UILabel *_lastPercentLabel;
	
	UILabel *_currencyLabel;
	UILabel *_timeLabel;
}

@property (nonatomic, retain) SymbolDynamicData *symbolDynamicData;

@property (nonatomic, retain) UILabel *tickerLabel;
@property (nonatomic, retain) UILabel *descriptionLabel;

@property (nonatomic, retain) UILabel *bidLabel;
@property (nonatomic, retain) UILabel *askLabel;
@property (nonatomic, retain) UILabel *lastLabel;
@property (nonatomic, retain) UILabel *lastChangeLabel;
@property (nonatomic, retain) UILabel *lastPercentLabel;
@property (nonatomic, retain) UILabel *currencyLabel;
@property (nonatomic, retain) UILabel *timeLabel;

@end