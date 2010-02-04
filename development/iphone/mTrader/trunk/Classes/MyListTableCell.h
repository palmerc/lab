//
//  MyListTableCell.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

@class Symbol;

@interface MyListTableCell : UITableViewCell {
	Symbol *symbol;
	
	IBOutlet UILabel *tickerLabel;
	IBOutlet UILabel *descriptionLabel;
	IBOutlet UIButton *centerButton;
	IBOutlet UIButton *rightButton;
	IBOutlet UILabel *timeLabel;
	
@private
	CGSize tickerLabelSize;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) IBOutlet UILabel *tickerLabel;
@property (nonatomic, retain) IBOutlet UILabel *descriptionLabel;
@property (nonatomic, retain) IBOutlet UIButton *centerButton;
@property (nonatomic, retain) IBOutlet UIButton *rightButton;
@property (nonatomic, retain) IBOutlet UILabel *timeLabel;

@end