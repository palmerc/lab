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
@class SymbolDetailController;

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
	SymbolDetailController *symbolDetail;
	
	NSFetchedResultsController *fetchedResultsController;
	NSManagedObjectContext *_managedObjectContext;
		
	CenterOptions centerOption;
	RightOptions rightOption;
	UIToolbar *_toolBar;
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) UIToolbar *toolBar;


- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

- (void)add:(id)sender;

- (void)deleteAllSymbols;
- (Feed *)fetchFeed:(NSString *)mCode;
- (Feed *)fetchFeedByName:(NSString *)feedName;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString	*)mCode;
- (NSArray *)fetchOrderBookForSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;

@end


