//
//  SymbolNewsController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;
@class NewsTableViewCell_Phone;


@interface SymbolNewsController_Phone : UITableViewController <NSFetchedResultsControllerDelegate> {
@private	
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	
	Symbol *_symbol;
	BOOL _newsAvailable;
	
	UILabel *_newsAvailableLabel;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UILabel *newsAvailableLabel;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

- (void)configureCell:(NewsTableViewCell_Phone *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end

