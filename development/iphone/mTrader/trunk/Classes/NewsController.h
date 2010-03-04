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

@interface NewsController : UIViewController <SymbolsDataDelegate, NSFetchedResultsControllerDelegate, UITableViewDelegate, UITableViewDataSource, UIPickerViewDelegate, UIPickerViewDataSource, UIActionSheetDelegate> {
@private
	mTraderCommunicator *communicator;
	NSManagedObjectContext *managedObjectContext;
	NSFetchedResultsController *fetchedResultsController;
	
	NSString *_mCode;	
	NSArray *newsArray;
	
	UITableView *table;
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSString *mCode;

- (void)configureCell:(NewsCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end
