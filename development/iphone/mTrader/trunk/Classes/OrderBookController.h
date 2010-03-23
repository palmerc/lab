//
//  OrderBookController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
#import "SymbolDataController.h"


@class Symbol;

@interface OrderBookController : UITableViewController <OrderBookDelegate> {
@private
	NSManagedObjectContext *_managedObjectContext;
	
	UIFont *tableFont;
	
	Symbol *_symbol;
	NSArray *_bidAsks;	
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSArray *bidAsks;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

@end

