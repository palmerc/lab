//
//  MyStocksViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
#import "StockSearchController.h"

@class Feed;
@class Symbol;

typedef enum {
	LAST_TRADE = 0,
	BID_PRICE,
	ASK_PRICE
} CenterButtonOptions;

typedef enum {
	LAST_TRADE_PERCENT_CHANGE = 0,
	LAST_TRADE_CHANGE,
	LAST_TRADE_TOO
} RightButtonOptions;

typedef enum {
	NOCHANGE = 0,
	UP,
	DOWN
} changeEnum;

@interface MyStocksViewController : UITableViewController <StockSearchControllerDelegate, SymbolsDataDelegate, NSFetchedResultsControllerDelegate> {
	mTraderCommunicator *communicator;
	
	NSUInteger currentValueType;
	
@private
	NSFetchedResultsController *fetchedResultsController;
	NSManagedObjectContext *managedObjectContext;
	
	CenterButtonOptions centerButtonOption;
	RightButtonOptions rightButtonOption;
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;

- (NSString *)sectionIndexTitleForSectionName:(NSString *)sectionName;
- (void)addStockButtonWasPressed:(id)sender;
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath;

- (void)centerButton:(id)sender;
- (void)rightButton:(id)sender;

- (Feed *)fetchFeed:(NSNumber *)feedNumber;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(Feed *)feed;

@end
