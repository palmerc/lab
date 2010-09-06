//
//  SymbolNewsController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;

@interface SymbolNewsController : UIViewController <NSFetchedResultsControllerDelegate, UITableViewDelegate, UITableViewDataSource> {
@private	
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;
	
	Symbol *_symbol;
	
	UIFont *_headlineFont;
	UIFont *_bottomlineFont;
	UILabel *_newsAvailableLabel;
	UITableView *_tableView;
	
	BOOL _modal;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property BOOL modal;

- (id)initWithSymbol:(Symbol *)symbol;

@end

