//
//  ChainsTableViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
#import "SymbolAddController.h"

@class NewsFeed;
@class Feed;
@class Symbol;
@class SymbolDetailController;

typedef enum {
	CLAST = 0,
	CBID,
	CASK
} CenterOptions;

typedef enum {
	RCHANGE_PERCENT = 0,
	RCHANGE,
	RLAST
} RightOptions;
//
//typedef enum {
//	NOCHANGE = 0,
//	UP,
//	DOWN
//} changeEnum;

@interface ChainsTableViewController : UITableViewController <SymbolsDataDelegate, NSFetchedResultsControllerDelegate> {
	NSUInteger currentValueType;
	
@private
	SymbolDetailController *symbolDetail;
	
	NSFetchedResultsController *_fetchedResultsController;
	NSManagedObjectContext *_managedObjectContext;
		
	CenterOptions centerOption;
	RightOptions rightOption;
	UIToolbar *_toolBar;
}

@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) UIToolbar *toolBar;


- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

- (void)centerSelection:(id)sender;
- (void)rightSelection:(id)sender;

- (void)deleteAllSymbols;
- (NewsFeed *)fetchNewsFeed:(NSString *)mCode;
- (Feed *)fetchFeed:(NSString *)mCode;
- (Feed *)fetchFeedByName:(NSString *)feedName;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString	*)mCode;

@end


