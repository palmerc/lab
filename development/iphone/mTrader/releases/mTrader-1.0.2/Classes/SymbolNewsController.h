//
//  SymbolNewsController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;
@class NewsTableViewCell_Phone;


@interface SymbolNewsController : UITableViewController <NSFetchedResultsControllerDelegate> {
@private	
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	
	Symbol *_symbol;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) Symbol *symbol;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

- (void)configureCell:(NewsTableViewCell_Phone *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end

