//
//  ChainsTableViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
#import "SymbolAddController.h"

@class Feed;
@class Symbol;

typedef enum {
	LAST_TRADE = 0,
	BID_PRICE,
	ASK_PRICE
} CenterOptions;

typedef enum {
	LAST_TRADE_PERCENT_CHANGE = 0,
	LAST_TRADE_CHANGE,
	LAST_TRADE_TOO
} RightOptions;

typedef enum {
	NOCHANGE = 0,
	UP,
	DOWN
} changeEnum;

@interface ChainsTableViewController : UITableViewController <SymbolAddControllerDelegate, SymbolsDataDelegate, NSFetchedResultsControllerDelegate> {
	mTraderCommunicator *communicator;
	
	NSUInteger currentValueType;
	
@private
	NSFetchedResultsController *fetchedResultsController;
	NSManagedObjectContext *managedObjectContext;
		
	CenterOptions centerOption;
	RightOptions rightOption;
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;

- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath;

- (void)add:(id)sender;
- (void)centerButton:(id)sender;
- (void)rightButton:(id)sender;

- (Feed *)fetchFeed:(NSString *)mCode;
- (Feed *)fetchFeedByName:(NSString *)feedName;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString	*)mCode;

@end
