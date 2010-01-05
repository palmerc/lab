//
//  StockListingCell.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface StockListingCell : UITableViewCell {
	IBOutlet UILabel *tickerLabel;
	IBOutlet UILabel *nameLabel;
	IBOutlet UIButton *valueButton;
}

@property (nonatomic, retain) IBOutlet UILabel *tickerLabel;
@property (nonatomic, retain) IBOutlet UILabel *nameLabel;
@property (nonatomic, retain) IBOutlet UIButton *valueButton;

@end
