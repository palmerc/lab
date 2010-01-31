//
//  StockListingCell.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol TouchedValueButtonDelegate;

@interface StockListingCell : UITableViewCell {
	id <TouchedValueButtonDelegate> _delegate;
	IBOutlet UILabel *tickerLabel;
	IBOutlet UILabel *nameLabel;
	IBOutlet UIButton *valueButton;
}

@property (nonatomic, assign) id <TouchedValueButtonDelegate> delegate;
@property (nonatomic, retain) IBOutlet UILabel *tickerLabel;
@property (nonatomic, retain) IBOutlet UILabel *nameLabel;
@property (nonatomic, retain) IBOutlet UIButton *valueButton;

-(IBAction) touchedValueButton:(id)sender;

@end

@protocol TouchedValueButtonDelegate <NSObject>
-(void) touchedValueButton:(id)sender;
@end;
