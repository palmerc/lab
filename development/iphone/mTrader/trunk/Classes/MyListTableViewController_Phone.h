//
//  MyListTableViewController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
#import "SymbolAddController_Phone.h"

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

@interface MyListTableViewController_Phone : UITableViewController <NSFetchedResultsControllerDelegate> {
	NSUInteger currentValueType;
	
@private
	CGRect _frame;
	SymbolDetailController *symbolDetail;
	
	NSFetchedResultsController *_fetchedResultsController;
	NSManagedObjectContext *_managedObjectContext;
	UINavigationController *_navigationController;
	
	CenterOptions centerOption;
	RightOptions rightOption;
	
	BOOL editing;
}

@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) UINavigationController *navigationController;

- (id)initWithFrame:(CGRect)frame;
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

- (void)toggleEditing;
- (void)centerSelection:(id)sender;
- (void)rightSelection:(id)sender;

@end


