//
//  NewsController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

#import "FeedsTableViewController.h"

@class NewsCell;
@class NewsFeed;
@class FeedsTableViewController;

@interface NewsController : UITableViewController <NSFetchedResultsControllerDelegate, NewsFeedChoiceDelegate> {
@private
	mTraderCommunicator *communicator;
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	NSFetchedResultsController *_feedsFetchedResultsController;
	
	UIBarButtonItem *feedBarButtonItem;
#ifdef UI_USER_INTERFACE_IDIOM
	UIPopoverController *_feedsPopover;
#endif
	FeedsTableViewController *feedsTableViewController;
	
	NewsFeed *_newsFeed;	
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSFetchedResultsController *feedsFetchedResultsController;
#ifdef UI_USER_INTERFACE_IDIOM
@property (nonatomic, retain) UIPopoverController *feedsPopover;
#endif
@property (nonatomic, retain) NewsFeed *newsFeed;

- (id)initWithMangagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)configureCell:(NewsCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (void)refresh:(id)sender;
@end
