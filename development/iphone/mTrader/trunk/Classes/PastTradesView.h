//
//  PastTradesView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 17.08.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface PastTradesView : UIView {
	UIFont *_headerFont;	
	UILabel *_timeLabel;
	UILabel *_priceLabel;
	UILabel *_volumeLabel;
	UILabel *_buyerLabel;
	UILabel *_sellerLabel;
	UITableView *_tableView;
	UILabel *_tradesAvailableLabel;
}

@property (nonatomic, retain) UILabel *timeLabel;
@property (nonatomic, retain) UILabel *priceLabel;
@property (nonatomic, retain) UILabel *volumeLabel;
@property (nonatomic, retain) UILabel *buyerLabel;
@property (nonatomic, retain) UILabel *sellerLabel;
@property (nonatomic, retain) UITableView *tableView;
@property (nonatomic, retain) UILabel *tradesAvailableLabel;

@end
