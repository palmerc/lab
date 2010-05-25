//
//  OrderBookView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangle.h"

@class OrderBookController;
@class OrderBookTableCellP;

@class Symbol;

@interface OrderBookView : RoundedRectangle {
@private	
	Symbol *_symbol;
	OrderBookController *_orderBookController;
	UIViewController *_viewController;

	UIButton *_orderBookButton;
	
	UILabel *_askSizeLabel;
	UILabel *_askValueLabel;
	UILabel *_bidSizeLabel;
	UILabel *_bidValueLabel;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UIViewController *viewController;
@property (nonatomic, retain) UIButton *orderBookButton;
@property (nonatomic, retain) UILabel *askSizeLabel;
@property (nonatomic, retain) UILabel *askValueLabel;
@property (nonatomic, retain) UILabel *bidSizeLabel;
@property (nonatomic, retain) UILabel *bidValueLabel;

- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

@end
