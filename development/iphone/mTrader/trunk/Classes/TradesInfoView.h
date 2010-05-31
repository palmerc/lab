//
//  TradesInfoView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 28.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangle.h"

@class Symbol;
@class TradesController;

@interface TradesInfoView : RoundedRectangle {
	Symbol *_symbol;
	
	TradesController *_tradesController;
	
	UIViewController *_viewController;
	UIButton *_tradesButton;
	UILabel *_timeLabel;
	UILabel *_buyerSellerLabel;
	UILabel *_priceLabel;
	UILabel *_sizeLabel;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UIViewController *viewController;
@property (nonatomic, retain) UIButton *tradesButton;
@property (nonatomic, retain) UILabel *timeLabel;
@property (nonatomic, retain) UILabel *buyerSellerLabel;
@property (nonatomic, retain) UILabel *priceLabel;
@property (nonatomic, retain) UILabel *sizeLabel;

- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

@end
