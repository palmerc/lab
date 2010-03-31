//
//  ChainsTableViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//


#import "ChainsTableViewController.h"

#import "ChainsTableCell.h"

#import "mTraderAppDelegate.h"
#import "mTraderCommunicator.h"
#import "QFields.h"
#import "SymbolDataController.h"

#import "NewsFeed.h"
#import "Feed.h";
#import "Symbol.h"
#import "SymbolDynamicData.h"

#import "SymbolDetailController.h"
#import "OrderBookController.h"

#import "StringHelpers.h"f
@implementation ChainsTableViewController
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize navigationController = _navigationController;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	if (self != nil) {
		editing = NO;
		self.managedObjectContext = managedObjectContext;
		_fetchedResultsController = nil;
	}
	return self;
}

#pragma mark -
#pragma mark Application lifecycle

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	// Core Data Setup - This not only grabs the existing results but also setups up the FetchController
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
}

/*
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return YES;
}
*/

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)viewDidUnload {
	self.fetchedResultsController = nil;
}

#pragma mark -
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	
	return [[self.fetchedResultsController sections] count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:section];
    return [sectionInfo numberOfObjects];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	NSInteger count = [[self.fetchedResultsController sections] count];
	if (count > 0) {
		id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:section];
		return [sectionInfo name];
	}
	return nil;	
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"ChainsTableCell";
    
    ChainsTableCell *cell = (ChainsTableCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[ChainsTableCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(ChainsTableCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	cell.centerOption = centerOption;
	cell.rightOption = rightOption;
	SymbolDynamicData *symbolDynamicData = (SymbolDynamicData *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	cell.symbolDynamicData = symbolDynamicData;
	
	if (animated == YES) {
		UIColor *flashColor = [UIColor yellowColor];
		UIColor *backgroundColor = [UIColor whiteColor];
		[cell.contentView setBackgroundColor:flashColor];
		[UIView beginAnimations:nil context:NULL];
		[UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
		[UIView setAnimationDuration:1.0];
		[cell.contentView setBackgroundColor:backgroundColor];
		[UIView commitAnimations];
	}
}

- (void)toggleEditing {
	if (editing == YES) {
		editing = NO;
		[self setEditing:NO animated:YES];
	} else {
		editing = YES;
		[self setEditing:YES animated:YES];
	}
}

#pragma mark -
#pragma mark TableViewDelegate methods

// This method is required to catch the swipe to delete gesture.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
	if (editingStyle == UITableViewCellEditingStyleDelete) {
		SymbolDynamicData *symbolDynamicData = (SymbolDynamicData *)[self.fetchedResultsController objectAtIndexPath:indexPath];
		
		[self.managedObjectContext deleteObject:symbolDynamicData.symbol];
		SymbolDataController *dataController = [SymbolDataController sharedManager];
		[dataController removeSymbol:symbolDynamicData.symbol];
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	// Request the latest static data and chart
	SymbolDynamicData *symbolDynamicData = (SymbolDynamicData *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	
	// Generate the view	
	symbolDetail = [[SymbolDetailController alloc] initWithSymbol:symbolDynamicData.symbol];
	
	// Give it the toolbar
	symbolDetail.managedObjectContext = self.managedObjectContext; 
	
	// Push the view onto the Navigation Controller
	[self.navigationController pushViewController:symbolDetail animated:YES];
	[symbolDetail release];
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

#pragma mark -
#pragma mark UIButton selectors

- (void)centerSelection:(id)sender {
	UIButton *button = (UIButton *)sender;
	
	switch (centerOption) {
		case CLAST:
			centerOption = CBID;
			[button setTitle:@"Bid" forState:UIControlStateNormal];
			break;
		case CBID:
			centerOption = CASK;
			[button setTitle:@"Ask" forState:UIControlStateNormal];
			break;
		case CASK:
			centerOption = CLAST;
			[button setTitle:@"Last" forState:UIControlStateNormal];
			break;
		default:
			break;
	}
	[self.tableView reloadData];
}

- (void)rightSelection:(id)sender {
	UIButton *button = (UIButton *)sender;
	switch (rightOption) {
		case RCHANGE_PERCENT:
			rightOption = RCHANGE;
			unichar upDownArrowsChar = 0x21C5;
			NSString *upDownArrows = [NSString stringWithCharacters:&upDownArrowsChar length:1];
			[button setTitle:upDownArrows forState:UIControlStateNormal];
			break;
		case RCHANGE:
			rightOption = RLAST;
			[button setTitle:@"Last" forState:UIControlStateNormal];
			break;
		case RLAST:
			rightOption = RCHANGE_PERCENT;
			[button setTitle:@"%" forState:UIControlStateNormal];
			break;
		default:
			break;
	}
	[self.tableView reloadData];
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
    
	// Create and configure a fetch request with the Book entity.
	NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"SymbolDynamicData" inManagedObjectContext:self.managedObjectContext];
	[fetchRequest setEntity:entity];
	
	// Create the sort descriptors array.
	NSSortDescriptor *tickerDescriptor = [[NSSortDescriptor alloc] initWithKey:@"symbol.index" ascending:YES];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:tickerDescriptor, nil];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
	self.fetchedResultsController = aFetchedResultsController;
	_fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[sortDescriptors release];
	
	return _fetchedResultsController;
}

/**
 Delegate methods of NSFetchedResultsController to respond to additions, removals and so on.
 */

- (void)controllerWillChangeContent:(NSFetchedResultsController *)controller {

	// The fetch controller is about to start sending change notifications, so prepare the table view for updates.
	[self.tableView beginUpdates];
}

- (void)controller:(NSFetchedResultsController *)controller didChangeObject:(id)anObject atIndexPath:(NSIndexPath *)indexPath forChangeType:(NSFetchedResultsChangeType)type newIndexPath:(NSIndexPath *)newIndexPath {
	
	UITableView *tableView = self.tableView;
	
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationFade];
			break;
			
		case NSFetchedResultsChangeDelete:
			[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
			break;
			
		case NSFetchedResultsChangeUpdate:
			[self configureCell:[tableView cellForRowAtIndexPath:indexPath] atIndexPath:indexPath animated:YES];
			break;
			
		case NSFetchedResultsChangeMove:
            [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
            [tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationFade];
            break;
    }
}

- (void)controller:(NSFetchedResultsController *)controller didChangeSection:(id <NSFetchedResultsSectionInfo>)sectionInfo atIndex:(NSUInteger)sectionIndex forChangeType:(NSFetchedResultsChangeType)type {
	
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[self.tableView insertSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationFade];
			break;
			
		case NSFetchedResultsChangeDelete:
			[self.tableView deleteSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationFade];
			break;
	}
}

- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
	// The fetch controller has sent all current change notifications, so tell the table view to process all updates.
	[self.tableView endUpdates];
}

//#pragma mark -
//#pragma mark Debugging methods
//// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	NSLog(@"Queried about %@ in ChainsTableViewController", NSStringFromSelector(sel));
//	return [super respondsToSelector:sel];
//}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_navigationController release];
	
	[_managedObjectContext release];
	[_fetchedResultsController release];
	
    [super dealloc];
}

@end
