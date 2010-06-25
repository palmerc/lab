//
//  NewsTableViewController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

#import "FeedsTableViewController_Phone.h"

@class NewsTableViewCell_Phone;
@class NewsFeed;
@class FeedsTableViewController_Phone;

@interface NewsTableViewContoller_Phone : UITableViewController <NSFetchedResultsControllerDelegate, NewsFeedChoiceDelegate> {
@private
	CGRect _frame;
	
	mTraderCommunicator *communicator;
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	NSFetchedResultsController *_feedsFetchedResultsController;
	
	UIBarButtonItem *feedBarButtonItem;
	FeedsTableViewController_Phone *feedsTableViewController;
	
	NewsFeed *_newsFeed;	
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSFetchedResultsController *feedsFetchedResultsController;
@property (nonatomic, retain) NewsFeed *newsFeed;

- (void)configureCell:(NewsTableViewCell_Phone *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (void)refresh:(id)sender;
@end
