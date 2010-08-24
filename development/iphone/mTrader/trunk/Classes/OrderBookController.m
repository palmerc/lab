//
//  OrderBookController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "OrderBookController.h"

#import "DataController.h"
#import "OrderBookView.h"
#import "OrderBookTableCellP.h"
#import "mTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "BidAsk.h"

#import "DataController.h"

@interface OrderBookController ()
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end

@implementation OrderBookController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize fetchedResultsController = _fetchedResultsController;

- (id)initWithSymbol:(Symbol *)symbol {
	self = [super init];
	if (self != nil) {

		_symbol = [symbol retain];
		
		_tableView = nil;
		_orderbookAvailableLabel = nil;
		_managedObjectContext = nil;		
	}
	return self;
}

- (void)loadView {
	_tableFont = [UIFont systemFontOfSize:14.0f];
	OrderBookView *orderBookView = [[OrderBookView alloc] initWithFrame:CGRectZero];
	orderBookView.tableView.delegate = self;
	orderBookView.tableView.dataSource = self;
	_tableView = [orderBookView.tableView retain];
	_orderbookAvailableLabel = [orderBookView.orderbookAvailableLabel retain];
	
	self.view = orderBookView;
	[orderBookView release];
}

#pragma mark -
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [[_fetchedResultsController sections] count];

}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	id <NSFetchedResultsSectionInfo> sectionInfo = [[_fetchedResultsController sections] objectAtIndex:section];
    NSUInteger rowCount = [sectionInfo numberOfObjects];
	
	if (rowCount == 0) {
		_orderbookAvailableLabel.hidden = NO;
	} else {
		_orderbookAvailableLabel.hidden = YES;
	}	
	return rowCount;
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"OrderBookTableCell_Phone";
    
    OrderBookTableCellP *cell = (OrderBookTableCellP *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[OrderBookTableCellP alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		cell.mainFont = _tableFont;
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
	}

    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(OrderBookTableCellP *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	BidAsk *bidAsk = (BidAsk *)[_fetchedResultsController objectAtIndexPath:indexPath];
	
	cell.bidAsk = bidAsk;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    CGSize size = [@"X" sizeWithFont:_tableFont];
	return size.height;
}

- (void)setManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	if (_managedObjectContext == managedObjectContext) {
		return;
	}
	[_managedObjectContext release];
	_managedObjectContext = [managedObjectContext retain];
	
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();  // Fail
#endif
	}	
}

#pragma mark -
#pragma mark Fetched results controller

/**
 Returns the fetched results controller. Creates and configures the controller if necessary.
 */
- (NSFetchedResultsController *)fetchedResultsController {	
    if (_fetchedResultsController != nil) {
        return _fetchedResultsController;
    }
    	
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"BidAsk" inManagedObjectContext:_managedObjectContext];
	NSFetchRequest *fetchRequest = [[[NSFetchRequest alloc] init] autorelease];
	[fetchRequest setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"symbol=%@", _symbol];
	[fetchRequest setPredicate:predicate];

	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"index" ascending:YES];
	[fetchRequest setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:_managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
	_fetchedResultsController = [aFetchedResultsController retain];
	_fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	
	return _fetchedResultsController;
}

/**
 Delegate methods of NSFetchedResultsController to respond to additions, removals and so on.
 */

- (void)controllerWillChangeContent:(NSFetchedResultsController *)controller {
	
	// The fetch controller is about to start sending change notifications, so prepare the table view for updates.
	[_tableView beginUpdates];
}

- (void)controller:(NSFetchedResultsController *)controller didChangeObject:(id)anObject atIndexPath:(NSIndexPath *)indexPath forChangeType:(NSFetchedResultsChangeType)type newIndexPath:(NSIndexPath *)newIndexPath {
	UITableView *tableView = _tableView;
	
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationNone];
			break;
			
		case NSFetchedResultsChangeUpdate:
			[self configureCell:[tableView cellForRowAtIndexPath:indexPath] atIndexPath:indexPath animated:YES];
			break;
			
		case NSFetchedResultsChangeDelete:
			[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationNone];
			break;
    }
}

- (void)controller:(NSFetchedResultsController *)controller didChangeSection:(id <NSFetchedResultsSectionInfo>)sectionInfo atIndex:(NSUInteger)sectionIndex forChangeType:(NSFetchedResultsChangeType)type {
	
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[_tableView insertSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationNone];
			break;
			
		case NSFetchedResultsChangeDelete:
			[_tableView deleteSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationNone];
			break;
	}
}

- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
	// The fetch controller has sent all current change notifications, so tell the table view to process all updates.
	[_tableView endUpdates];
}

#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
#if DEBUG
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in OrderBookController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_tableFont release];
	[_symbol release];
	[_orderbookAvailableLabel release];
	[_tableView release];	
	
	[_fetchedResultsController release];
	[_managedObjectContext release];
    [super dealloc];
}

@end

