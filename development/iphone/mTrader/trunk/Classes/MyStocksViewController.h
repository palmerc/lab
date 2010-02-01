//
//  MyStocksViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>
#import "mTraderCommunicator.h"
#import "StockListingCell.h"
#import "StockSearchController.h"

@class mTraderCommunicator;
@class Feed;
@class Symbol;
typedef enum {
	NOCHANGE = 0,
	UP,
	DOWN
} changeEnum;

typedef enum {
	PRICE = 0,
	PERCENT,
	VOLUME
} valueType;

@interface MyStocksViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, StockSearchControllerDelegate, SymbolsDataDelegate, NSFetchedResultsControllerDelegate> {
	mTraderCommunicator *communicator;
	
	NSFetchedResultsController *fetchedResultsController;
	NSManagedObjectContext *managedObjectContext;

	NSUInteger currentValueType;
	BOOL _editing;
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;

@property (assign) BOOL editing;

- (void)addStockButtonWasPressed:(id)sender;
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath;
- (Feed *)fetchFeed:(NSNumber *)feedNumber;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(Feed *)feed;

@end
