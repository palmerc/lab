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
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) TradesController *tradesController;
@property (nonatomic, retain) UIViewController *viewController;
@property (nonatomic, retain) UIButton *tradesButton;

- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

@end
