//
//  OrderBookController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
#import "DataController.h"


@class Symbol;

@interface OrderBookController : UIViewController <OrderBookDelegate, UITableViewDelegate, UITableViewDataSource> {
@private
	NSManagedObjectContext *_managedObjectContext;
	
	Symbol *_symbol;
	NSArray *_bidAsks;
	
	UIFont *_tableFont;
	UILabel *_orderbookAvailableLabel;
	UITableView *_tableView;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;

- (id)initWithSymbol:(Symbol *)symbol;

@end