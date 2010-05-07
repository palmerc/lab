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

@interface NewsController : UITableViewController <NSFetchedResultsControllerDelegate, UIActionSheetDelegate, NewsFeedChoiceDelegate> {
@private
	mTraderCommunicator *communicator;
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	NSFetchedResultsController *_feedsFetchedResultsController;
	
	UIBarButtonItem *feedBarButtonItem;
	UIPopoverController *_feedsPopover;
	FeedsTableViewController *feedsTableViewController;
	
	NewsFeed *_newsFeed;	
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSFetchedResultsController *feedsFetchedResultsController;
@property (nonatomic, retain) UIPopoverController *feedsPopover;
@property (nonatomic, retain) NewsFeed *newsFeed;

- (id)initWithMangagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)configureCell:(NewsCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (void)refresh:(id)sender;
@end
