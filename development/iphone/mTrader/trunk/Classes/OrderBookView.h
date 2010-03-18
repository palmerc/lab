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
	OrderBookController *orderBookController;
}

@property (nonatomic, retain) Symbol *symbol;

- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)configureCell:(OrderBookTableCellP *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

@end
