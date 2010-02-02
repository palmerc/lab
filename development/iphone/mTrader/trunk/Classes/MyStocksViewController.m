//
//  MyStocksViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//
#import "MyStocksViewController.h"

#import "mTraderAppDelegate.h"
#import "mTraderCommunicator.h"

#import "Feed.h";
#import "Symbol.h"

#import "StockDetailController.h"
#import "StockSearchController.h"
#import "StockListingCell.h"

#import "StringHelpers.h"

@implementation MyStocksViewController
@synthesize communicator;
@synthesize fetchedResultsController, managedObjectContext;
@synthesize editing = _editing;

#pragma mark -
#pragma mark Application lifecycle

- (id)init {
	self = [super init];
	if (self != nil) {
		self.title = NSLocalizedString(@"MyStocksTab", @"My Stocks tab label");
		//UIImage* anImage = [UIImage imageNamed:@"myStocksTabButton.png"];
		//UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"MyStocksTab", @"My Stocks tab label") image:anImage tag:MYSTOCKS];
		//self.tabBarItem = theItem;
		//[theItem release];
				
		//_symbolsController = [SymbolsController sharedManager];
		//_symbolsController = nil;
		
		currentValueType = PRICE;
		self.editing = NO;
	}
	return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	// Core Data Setup - This not only grabs the existing results but also setups up the FetchController
	NSError *error;
	if (![[self fetchedResultsController] performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}

	// Setup right and left bar buttons
	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(addStockButtonWasPressed:)];
	self.navigationItem.rightBarButtonItem = addItem;
	[addItem release];
	self.navigationItem.leftBarButtonItem = self.editButtonItem;
	
	// Establish the delegation of incoming symbols to be given to us.
	self.communicator = [mTraderCommunicator sharedManager];
	self.communicator.symbolsDelegate = self;
}

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return YES;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	self.fetchedResultsController = nil;
}


#pragma mark -
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	
	return [[fetchedResultsController sections] count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	id <NSFetchedResultsSectionInfo> sectionInfo = [[fetchedResultsController sections] objectAtIndex:section];
    return [sectionInfo numberOfObjects];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	NSInteger count = [[fetchedResultsController sections] count];
	if (count > 0) {
		id <NSFetchedResultsSectionInfo> sectionInfo = [[fetchedResultsController sections] objectAtIndex:section];
		return [sectionInfo name];
	}
	return nil;	
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
    }
    
    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath];
    return cell;
}

- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath {
    // Configure the cell to show the book's title
	Symbol *symbol = [fetchedResultsController objectAtIndexPath:indexPath];
	cell.textLabel.text = symbol.tickerSymbol;
	cell.detailTextLabel.text = symbol.companyName;
}

/*
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	static NSString *CellIdentifier = @"SymbolCell";
	
	StockListingCell *cell = (StockListingCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	if (cell == nil) {
		NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:@"StockListingCell" owner:nil options:nil];
		for (id currentObject in topLevelObjects) {
			if ([currentObject isKindOfClass:[StockListingCell class]]) {
				cell = (StockListingCell *)currentObject;
				cell.delegate = self;
				break;
			}
		}
	}
	
	Symbol *symbol = [self.symbolsController symbolAtIndexPath:indexPath];
	/*
	changeEnum changeType;
	if ([symbol.changeSinceLastUpdate floatValue] < 0) {
		changeType = DOWN;
	} else if ([symbol.changeSinceLastUpdate floatValue] > 0) {
		changeType = UP;
	} else {
		changeType = NOCHANGE;
	}

	BOOL animateChange = NO;
	UIColor *flashColor = nil;
	if (changeType != NOCHANGE) {
		switch (changeType) {
			case UP:
				animateChange = YES;
				flashColor = [UIColor greenColor];
				break;
			case DOWN:
				animateChange = YES;
				flashColor = [UIColor redColor];
				break;
			default:
				animateChange = NO;
				break;
		}
	}
	
	if (animateChange) {
		UIColor *backgroundColor = [UIColor whiteColor];
		[cell.contentView setBackgroundColor:flashColor];
		[UIView beginAnimations:nil context:NULL];
		[UIView setAnimationDuration:0.5];
		[cell.contentView setBackgroundColor:backgroundColor];
		[UIView commitAnimations];
	}
	*/
	//cell.editing = YES;
/*
	cell.tickerLabel.text = symbol.tickerSymbol;
	cell.nameLabel.text = symbol.name;
	
	NSString *title;
	switch (currentValueType) {
		case PRICE:
			title = symbol.lastTrade;
			break;
		case PERCENT:
			title = symbol.percentChange;	
			break;
		case VOLUME:
			title = symbol.volume;
			break;
		default:
			title = nil;
			break;
	}
	
	[cell.valueButton setTitle:title forState:UIControlStateNormal];
	
	return cell;
}
*/
#pragma mark -
#pragma mark TableViewDelegate methods
/*
// This method is required to catch the swipe to delete gesture.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
	if (editingStyle == UITableViewCellEditingStyleDelete) {
		NSManagedObject *eventToDelete = [stocksArray objectAtIndex:indexPath.row];
		[managedObjectContext deleteObject:eventToDelete];
		
		[stocksArray removeObjectAtIndex:indexPath.row];
		
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
		NSError *error;
		if (![managedObjectContext save:&error]) {
			// Handle the error.
		}
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }	
}

- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
}
*/
#pragma mark -
#pragma mark Delegation
/**
 * Delegation
 */

-(void) addSymbols:(NSString *)symbols {
	static NSInteger FEED_TICKER = 0;
	static NSInteger TICKER_SYMBOL = 1;
	static NSInteger COMPANY_NAME = 2;
	static NSInteger EXCHANGE_CODE = 3;
	static NSInteger TYPE = 4;
	static NSInteger ORDER_BOOK = 5;
	static NSInteger ISIN = 6;
	
	// insert the objects
	NSArray *rows = [symbols componentsSeparatedByString:@":"];	
	for (NSString *row in rows) {
		NSArray *stockComponents = [row componentsSeparatedByString:@";"];
		stockComponents = [StringHelpers cleanComponents:stockComponents];
		NSString *feedTicker = [stockComponents objectAtIndex:FEED_TICKER];
		NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
		NSString *feedNumberString = [feedTickerComponents objectAtIndex:0];
		NSNumber *feedNumber = [NSNumber numberWithInteger:[feedNumberString integerValue]];
		//NSString *ticker = [feedTickerComponents objectAtIndex:1];
		NSString *tickerSymbol = [stockComponents objectAtIndex:TICKER_SYMBOL];
		NSString *companyName = [stockComponents objectAtIndex:COMPANY_NAME];
		NSString *exchangeCode = [stockComponents objectAtIndex:EXCHANGE_CODE];
		NSString *orderBook = [stockComponents objectAtIndex:ORDER_BOOK];
		NSString *type = [stockComponents objectAtIndex:TYPE];
		NSString *isin = [stockComponents objectAtIndex:ISIN];
		
		// Prevent double insertions
		Feed *feed = [self fetchFeed:feedNumber];
		if (feed == nil) {
			feed = (Feed *)[NSEntityDescription insertNewObjectForEntityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
			feed.mCode = exchangeCode;
			feed.description = exchangeCode;
			feed.feedNumber = feedNumber;
		}
		
		Symbol *symbol = [self fetchSymbol:tickerSymbol withFeed:feed]; 
		if (symbol == nil) {
			symbol = (Symbol *)[NSEntityDescription insertNewObjectForEntityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
			symbol.tickerSymbol = tickerSymbol;
			symbol.companyName = companyName;
			symbol.orderBook = orderBook;
			symbol.type = type;
			symbol.isin = isin;
			[feed addSymbolsObject:symbol];
		}

	}
	// save the objects
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		// Handle the error
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
}

/**
 * This method should receive a list of symbols that have been updated and should
 * update any rows necessary.
 */
/*
- (void)symbolsUpdated:(NSArray *)feedTickers {
	if (!self.editing) {
		NSMutableArray *indexPaths = [[NSMutableArray alloc] init];

		for (NSString *feedTicker in feedTickers) {
			NSIndexPath *indexPath = [self.symbolsController indexPathOfSymbol:feedTicker];
			[indexPaths addObject:indexPath];
		}
		
		[self.tableView beginUpdates];
		[self.tableView reloadRowsAtIndexPaths:indexPaths withRowAnimation:UITableViewRowAnimationNone];
		[self.tableView endUpdates];
		[indexPaths release];
	}
}
*/
#pragma mark -
#pragma mark UI Actions
/*
- (void)addStockButtonWasPressed:(id)sender {
	StockSearchController *controller = [[StockSearchController alloc] initWithNibName:@"StockSearchView" bundle:nil];
	
	controller.delegate = self;

	controller.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
	[self presentModalViewController:controller animated:YES];
	
	[controller release];
}

- (void)stockSearchControllerDidFinish:(StockSearchController *)stockSearchController didAddSymbol:(NSString *)tickerSymbol {
	[self dismissModalViewControllerAnimated:YES];
}

-(void) touchedValueButton:(id)sender {
	switch (currentValueType) {
		case PRICE:
			currentValueType = PERCENT;
			break;
		case PERCENT:
			currentValueType = VOLUME;
			break;
		case VOLUME:
			currentValueType = PRICE;
			break;
		default:
			break;
	}
	[self.tableView reloadData];
}
*/
#pragma mark -
#pragma mark Fetched results controller

/**
 Returns the fetched results controller. Creates and configures the controller if necessary.
 */
- (NSFetchedResultsController *)fetchedResultsController {
	
    if (fetchedResultsController != nil) {
        return fetchedResultsController;
    }
    
	// Create and configure a fetch request with the Book entity.
	NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:managedObjectContext];
	[fetchRequest setEntity:entity];
	
	// Create the sort descriptors array.
	NSSortDescriptor *feedDescriptor = [[NSSortDescriptor alloc] initWithKey:@"feed.mCode" ascending:YES];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:feedDescriptor, nil];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:managedObjectContext sectionNameKeyPath:@"feed.mCode" cacheName:@"Root"];
	self.fetchedResultsController = aFetchedResultsController;
	fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[feedDescriptor release];
	[sortDescriptors release];
	
	return fetchedResultsController;
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
			[self configureCell:[tableView cellForRowAtIndexPath:indexPath] atIndexPath:indexPath];
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

- (Feed *)fetchFeed:(NSNumber *)feedNumber {
	NSManagedObjectContext *moc = [self managedObjectContext];
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Feed" inManagedObjectContext:moc];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feedNumber=%@)", feedNumber];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"mCode" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [moc executeFetchRequest:request error:&error];
	if (array == nil)
	{
		// Deal with error...
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(Feed *)feed {
	NSManagedObjectContext *moc = [self managedObjectContext];
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:moc];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed=%@) AND (tickerSymbol=%@)", feed, tickerSymbol];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [moc executeFetchRequest:request error:&error];
	if (array == nil)
	{
		// Deal with error...
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

#pragma mark -
#pragma mark Debugging methods

/*
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
    NSLog(@"Queried about %@", NSStringFromSelector(sel));
    return [super respondsToSelector:sel];
}
*/

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [super dealloc];
}

@end
