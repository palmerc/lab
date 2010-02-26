//
//  NewsViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@interface NewsViewController : UITableViewController <SymbolsDataDelegate, UIPickerViewDataSource, UIPickerViewDelegate, NSFetchedResultsControllerDelegate, UIActionSheetDelegate> {
	mTraderCommunicator *communicator;
	
	NSManagedObjectContext *managedObjectContext;
	NSFetchedResultsController *fetchedResultsController;
	NSMutableArray *_newsArray;
	
	NSString *mCode;
}
@property (nonatomic, assign) mTraderCommunicator *communicator;

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSMutableArray *newsArray;

-(void) refreshNews;
@end
