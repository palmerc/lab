//
//  SymbolNewsModalController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 24.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;
@class SymbolNewsTableViewCell_Phone;

@protocol SymbolNewsModalControllerDelegate;

@interface SymbolNewsModalController : UITableViewController <NSFetchedResultsControllerDelegate> {
@private
	id <SymbolNewsModalControllerDelegate> delegate;
	
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	
	Symbol *_symbol;
}

@property (assign) id <SymbolNewsModalControllerDelegate> delegate;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) Symbol *symbol;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)refresh:(id)sender;

- (void)configureCell:(SymbolNewsTableViewCell_Phone *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end

@protocol SymbolNewsModalControllerDelegate
- (void)symbolNewsModalControllerDidFinish:(SymbolNewsModalController *)controller;
@end