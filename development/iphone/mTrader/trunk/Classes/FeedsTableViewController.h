//
//  FeedsTableViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 04.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NewsFeed;
@protocol NewsFeedChoiceDelegate;

@interface FeedsTableViewController : UITableViewController <NSFetchedResultsControllerDelegate> {
	id <NewsFeedChoiceDelegate> delegate;
	
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	
	NSIndexPath *previousChoice;
}

@property (nonatomic, assign) id <NewsFeedChoiceDelegate> delegate;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@end

@protocol NewsFeedChoiceDelegate <NSObject>
- (void)newsFeedWasSelected:(NewsFeed *)newsFeed;
@end