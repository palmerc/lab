//
//  OrderBookController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "DataController.h"

@class Symbol;

@interface OrderBookController : UIViewController <NSFetchedResultsControllerDelegate, UITableViewDelegate, UITableViewDataSource> {
@private
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	
	Symbol *_symbol;
	
	UIFont *_tableFont;
	UILabel *_orderbookAvailableLabel;
	UITableView *_tableView;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;

- (id)initWithSymbol:(Symbol *)symbol;

@end