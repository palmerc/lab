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
#import "Chart.h"

#import "SymbolDetailController.h"
#import "OrderBookController.h"

#import "StringHelpers.h"

@implementation ChainsTableViewController
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize toolBar = _toolBar;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	if (self != nil) {
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

	self.title = NSLocalizedString(@"ChainsTab", @"Chains tab label");

	// Core Data Setup - This not only grabs the existing results but also setups up the FetchController
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
	
	self.navigationItem.leftBarButtonItem = self.editButtonItem;
	
	// Setup right and left bar buttons
	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(add:)];
	self.navigationItem.rightBarButtonItem = addItem;
	[addItem release];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return YES;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)viewDidUnload {
	self.fetchedResultsController = nil;
	self.toolBar = nil;
}

- (void)viewWillAppear:(BOOL)animated {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	if ([communicator isLoggedIn]) {
		QFields *qFields = [[QFields alloc] init];
		qFields.timeStamp = YES;
		qFields.lastTrade = YES;
		qFields.bidPrice = YES;
		qFields.askPrice = YES;
		qFields.change = YES;
		qFields.changePercent = YES;
		communicator.qFields = qFields;
		[qFields release];
		[communicator setStreamingForFeedTicker:nil];
	}
	
	NSArray *centerItems = [NSArray arrayWithObjects:@"Last", @"Bid", @"Ask", nil];
	UISegmentedControl *centerControl = [[UISegmentedControl alloc] initWithItems:centerItems];
	centerControl.segmentedControlStyle = UISegmentedControlStyleBar;
	centerControl.selectedSegmentIndex = 0;
	[centerControl addTarget:self action:@selector(centerSelection:) forControlEvents:UIControlEventValueChanged];
	
	unichar upDownArrowsChar = 0x21C5;
	NSString *upDownArrows = [NSString stringWithCharacters:&upDownArrowsChar length:1];
	NSArray *rightItems = [NSArray arrayWithObjects: @"%", upDownArrows, @"Last", nil];
	UISegmentedControl *rightControl = [[UISegmentedControl alloc] initWithItems:rightItems];
	rightControl.segmentedControlStyle = UISegmentedControlStyleBar;
	rightControl.selectedSegmentIndex = 0;
	[rightControl addTarget:self action:@selector(rightSelection:) forControlEvents:UIControlEventValueChanged];
	
	UIBarButtonItem *centerBarItem = [[UIBarButtonItem alloc] initWithCustomView:centerControl];
	UIBarButtonItem *rightBarItem = [[UIBarButtonItem alloc] initWithCustomView:rightControl];
	[centerControl release];
	[rightControl release];
	[self.toolBar setItems:[NSArray arrayWithObjects:centerBarItem, rightBarItem, nil]];
	[centerBarItem release];
	[rightBarItem release];
	
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
//	cell.centerOption = centerOption;
//	cell.rightOption = rightOption;
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
	symbolDetail.toolBar = self.toolBar;
	symbolDetail.managedObjectContext = self.managedObjectContext; 
	
	// Push the view onto the Navigation Controller
	[self.navigationController pushViewController:symbolDetail animated:YES];
	[symbolDetail release];
}

#pragma mark -
#pragma mark UIButton selectors

//- (void)centerSelection:(id)sender {
//	UISegmentedControl *control = sender;
//	switch (control.selectedSegmentIndex) {
//		case LAST_TRADE:
//			centerOption = LAST_TRADE;
//			break;
//		case BID_PRICE:
//			centerOption = BID_PRICE;
//			break;
//		case ASK_PRICE:
//			centerOption = ASK_PRICE;
//			break;
//		default:
//			break;
//	}
//	[self.tableView reloadData];
//}
//
//- (void)rightSelection:(id)sender {
//	UISegmentedControl *control = sender;
//	switch (control.selectedSegmentIndex) {
//		case LAST_TRADE_PERCENT_CHANGE:
//			rightOption = LAST_TRADE_PERCENT_CHANGE;
//			break;
//		case LAST_TRADE_CHANGE:
//			rightOption = LAST_TRADE_CHANGE;
//			break;
//		case LAST_TRADE_TOO:
//			rightOption = LAST_TRADE_TOO;
//			break;
//		default:
//			break;
//	}
//	[self.tableView reloadData];
//}



- (void)failedToAddAlreadyExists {
	NSString *alertTitle = @"Add Security Failed";
	NSString *alertMessage = @"The ticker symbol you requested is already in your list.";
	NSString *alertCancel = @"Dismiss";
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertTitle message:alertMessage delegate:nil cancelButtonTitle:alertCancel otherButtonTitles:nil];
	[alertView show];	
}

- (void)failedToAddNoSuchSecurity {
	NSString *alertTitle = @"Add Security Failed";
	NSString *alertMessage = @"The ticker symbol you requested was not found.";
	NSString *alertCancel = @"Dismiss";
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertTitle message:alertMessage delegate:nil cancelButtonTitle:alertCancel otherButtonTitles:nil];
	[alertView show];
}


#pragma mark -
#pragma mark UI Actions
- (void)add:(id)sender {
	SymbolAddController *controller = [[SymbolAddController alloc] initWithNibName:@"SymbolAddView" bundle:nil];
	controller.delegate = self;
	controller.managedObjectContext = self.managedObjectContext;
	
	UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:controller];
	[controller release];
	[self.parentViewController presentModalViewController:navigationController animated:YES];
	[navigationController release];
}

- (void)symbolAddControllerDidFinish:(SymbolAddController *)stockSearchController didAddSymbol:(NSString *)tickerSymbol {
	[self dismissModalViewControllerAnimated:YES];
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

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_managedObjectContext release];
	[_fetchedResultsController release];
	
	[self.toolBar release];
    [super dealloc];
}

@end
