//
//  PastTradesController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "mTraderCommunicator.h"

@class Symbol;

@interface PastTradesController : UIViewController <NSFetchedResultsControllerDelegate, UITableViewDelegate, UITableViewDataSource> {
@private
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	Symbol *_symbol;
	
	UITableView *_tableView;
	UILabel *_tradesAvailableLabel;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;

- (id)initWithSymbol:(Symbol *)symbol;

@end

