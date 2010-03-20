//
//  NewsController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@class NewsCell;
@protocol NewsControllerDelegate;

@interface NewsController : UIViewController <NSFetchedResultsControllerDelegate, UITableViewDelegate, UITableViewDataSource, UIPickerViewDelegate, UIPickerViewDataSource, UIActionSheetDelegate> {
@private
	mTraderCommunicator *communicator;
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	NSFetchedResultsController *_feedsFetchedResultsController;
	
	NSString *_mCode;	
	
	UITableView *table;
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSFetchedResultsController *feedsFetchedResultsController;
@property (nonatomic, retain) NSString *mCode;

- (id)initWithMangagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)configureCell:(NewsCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end
