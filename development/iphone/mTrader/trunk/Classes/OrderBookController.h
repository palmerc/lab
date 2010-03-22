//
//  OrderBookController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
@protocol OrderBookControllerDelegate;
@class Symbol;

@interface OrderBookController : UIViewController <UITableViewDelegate, UITableViewDataSource> {
@private
	id <OrderBookControllerDelegate> delegate;

	NSManagedObjectContext *_managedObjectContext;
	
	UIFont *tableFont;
	
	Symbol *_symbol;
	NSArray *_bidAsks;	
		
	UITableView *table;
}

@property (assign) id <OrderBookControllerDelegate> delegate;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSArray *bidAsks;
@property (nonatomic, retain) UITableView *table;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)updateSymbol;
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

@end