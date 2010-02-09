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

#import "Feed.h";
#import "Symbol.h"
#import "SymbolDynamicData.h"

#import "StockDetailController.h"

#import "StringHelpers.h"

@implementation ChainsTableViewController
@synthesize communicator;
@synthesize fetchedResultsController, managedObjectContext;

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
	
	// Add the toolbar to the bottom.
	UIToolbar *toolBar = [[UIToolbar alloc] init];
	[toolBar sizeToFit];
	toolBar.barStyle = UIBarStyleDefault;
	CGRect frame = self.view.frame;
	CGFloat x = 0;
	CGFloat width = CGRectGetWidth(frame);
	CGFloat height = toolBar.frame.size.height;
	CGFloat y = self.tableView.frame.size.height - height;
	CGRect newFrame = CGRectMake(x, y, width, height);	
	toolBar.frame = newFrame;
		
	//CGRect tableViewBounds = self.tableView.bounds;
//	CGFloat tableViewHeight = self.tableView.bounds.size.height;
//	tableViewBounds.size.height = tableViewHeight - height;
//	self.tableView.bounds = tableViewBounds;
	
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
	[toolBar setItems:[NSArray arrayWithObjects:centerBarItem, rightBarItem, nil]];
		
	[self.parentViewController.view addSubview:toolBar];
	[toolBar release];
	
	// Setup right and left bar buttons
	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(add:)];
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
    static NSString *CellIdentifier = @"ChainsTableCell";
    
    ChainsTableCell *cell = (ChainsTableCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[ChainsTableCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath];
    return cell;
}

- (void)configureCell:(ChainsTableCell *)cell atIndexPath:(NSIndexPath *)indexPath {
	cell.centerOption = centerOption;
	cell.rightOption = rightOption;
	SymbolDynamicData *symbolDynamicData = (SymbolDynamicData *)[fetchedResultsController objectAtIndexPath:indexPath];
	cell.symbolDynamicData = symbolDynamicData;
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

// This method is required to catch the swipe to delete gesture.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
	if (editingStyle == UITableViewCellEditingStyleDelete) {
		SymbolDynamicData *symbolDynamicData = (SymbolDynamicData *)[fetchedResultsController objectAtIndexPath:indexPath];
		NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", symbolDynamicData.symbol.feed.feedNumber, symbolDynamicData.symbol.tickerSymbol];
		[communicator removeSecurity:feedTicker];
		
		[self.managedObjectContext deleteObject:symbolDynamicData.symbol];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }	
}

- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
}

#pragma mark -
#pragma mark UIButton selectors

- (void)centerSelection:(id)sender {
	UISegmentedControl *control = sender;
	switch (control.selectedSegmentIndex) {
		case LAST_TRADE:
			centerOption = LAST_TRADE;
			break;
		case BID_PRICE:
			centerOption = BID_PRICE;
			break;
		case ASK_PRICE:
			centerOption = ASK_PRICE;
			break;
		default:
			break;
	}
	[self.tableView reloadData];
}

- (void)rightSelection:(id)sender {
	UISegmentedControl *control = sender;
	switch (control.selectedSegmentIndex) {
		case LAST_TRADE_PERCENT_CHANGE:
			rightOption = LAST_TRADE_PERCENT_CHANGE;
			break;
		case LAST_TRADE_CHANGE:
			rightOption = LAST_TRADE_CHANGE;
			break;
		case LAST_TRADE_TOO:
			rightOption = LAST_TRADE_TOO;
			break;
		default:
			break;
	}
	[self.tableView reloadData];
}


#pragma mark -
#pragma mark Delegation
/**
 * Delegation
 */

- (void)addExchanges:(NSArray *)exchanges {
	exchanges = [StringHelpers cleanComponents:exchanges];
	for (NSString *exchangeCode in exchanges) {
		
		// Separate the Description from the mCode
		NSRange leftBracketRange = [exchangeCode rangeOfString:@"["];
		NSRange rightBracketRange = [exchangeCode rangeOfString:@"]"];
		NSRange leftParenthesisRange = [exchangeCode rangeOfString:@"("];
		NSRange rightParenthesisRange = [exchangeCode rangeOfString:@")"];
		
		NSRange typeRange;
		typeRange.location = leftParenthesisRange.location + 1;
		typeRange.length = rightParenthesisRange.location - typeRange.location;
		NSString *typeCode = [exchangeCode substringWithRange:typeRange]; // (S) 
		
		NSRange mCodeRange;
		mCodeRange.location = leftBracketRange.location + 1;
		mCodeRange.length = rightBracketRange.location - mCodeRange.location;
		NSString *mCode = [exchangeCode substringWithRange:mCodeRange]; // OSS
		
		NSRange descriptionRange;
		descriptionRange.location = 0;
		descriptionRange.length = leftBracketRange.location - 1;
		NSString *feedName = [exchangeCode substringWithRange:descriptionRange]; // Oslo Stocks
		
		Feed *feed = [self fetchFeedByName:feedName];
		if (feed == nil) {
			feed = (Feed *)[NSEntityDescription insertNewObjectForEntityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
		}
		
		feed.mCode = mCode;
		feed.feedName = feedName;
		feed.typeCode = typeCode;
	}
	
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		// Handle the error
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();
	}
}

- (void)addSymbols:(NSString *)symbols {
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
		
		// Separate the Description from the mCode
		NSRange leftBracketRange = [exchangeCode rangeOfString:@"["];
		NSRange rightBracketRange = [exchangeCode rangeOfString:@"]"];
		
		NSRange mCodeRange;
		mCodeRange.location = leftBracketRange.location + 1;
		mCodeRange.length = rightBracketRange.location - mCodeRange.location;
		NSString *mCode = [exchangeCode substringWithRange:mCodeRange]; // OSS
		
		NSRange descriptionRange;
		descriptionRange.location = 0;
		descriptionRange.length = leftBracketRange.location - 1;
		NSString *feedName = [exchangeCode substringWithRange:descriptionRange]; // Oslo Stocks
	
		// Prevent double insertions
		Feed *feed = [self fetchFeed:mCode];
		if (feed == nil) {
			feed = (Feed *)[NSEntityDescription insertNewObjectForEntityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
			feed.mCode = mCode; // OSS
			feed.feedName = feedName;	// Oslo Stocks
			feed.typeCode = @"";
		}
		
		feed.feedNumber = feedNumber; // 18177
		
		Symbol *symbol = [self fetchSymbol:tickerSymbol withFeed:mCode]; 
		if (symbol == nil) {
			symbol = (Symbol *)[NSEntityDescription insertNewObjectForEntityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
			symbol.tickerSymbol = tickerSymbol;
			symbol.companyName = companyName;
			symbol.country = nil;
			symbol.currency = nil;
			symbol.orderBook = orderBook;
			symbol.type = type;
			symbol.isin = isin;
			symbol.symbolDynamicData = (SymbolDynamicData *)[NSEntityDescription insertNewObjectForEntityForName:@"SymbolDynamicData" inManagedObjectContext:self.managedObjectContext];

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
- (void)updateSymbols:(NSArray *)updates {
	static NSInteger FEED_TICKER = 0;
	static NSInteger LAST_TRADE = 1;
	static NSInteger PERCENT_CHANGE = 2;
	static NSInteger BID_PRICE = 3;
	static NSInteger ASK_PRICE = 4;
	static NSInteger ASK_VOLUME = 5;
	static NSInteger BID_VOLUME = 6;
	static NSInteger CHANGE = 7;
	static NSInteger HIGH = 8;
	static NSInteger LOW = 9;
	static NSInteger OPEN = 10;
	static NSInteger VOLUME = 11;

	for (NSString *update in updates) {
		
		NSArray *values = [StringHelpers cleanComponents:[update componentsSeparatedByString:@";"]];

		NSString *feedTicker = [values objectAtIndex:FEED_TICKER];
		NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
		NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
		NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
		
		NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
		NSEntityDescription *entity = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
		[request setEntity:entity];
		
		NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed.feedNumber=%@) AND (tickerSymbol=%@)", feedNumber, tickerSymbol];
		[request setPredicate:predicate];
		
		NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
		[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
		[sortDescriptor release];
		
		NSError *error = nil;
		NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
		if (array == nil)
		{
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		}
				
		Symbol *symbol = [array objectAtIndex:0];
		SymbolDynamicData *symbolDynamicData = symbol.symbolDynamicData;
		
		// last trade
		if ([values count] > LAST_TRADE) {
			NSString *lastTrade = [values objectAtIndex:LAST_TRADE];
			if ([lastTrade isEqualToString:@""] == NO) {
				symbolDynamicData.lastTrade = [NSNumber numberWithDouble:[lastTrade doubleValue]];
				symbolDynamicData.lastTradeTime = [NSDate date];
			}
		}
		
		// percent change
		if ([values count] > PERCENT_CHANGE) {
			NSString *percentChange = [values objectAtIndex:PERCENT_CHANGE];
			if ([percentChange isEqualToString:@""] == NO) {
				symbolDynamicData.lastTradePercentChange = [NSNumber numberWithDouble:([percentChange doubleValue]/100.0)];
			}
		}
		
		// bid price
		if ([values count] > BID_PRICE) {
			NSString *bidPrice = [values objectAtIndex:BID_PRICE];
			if ([bidPrice isEqualToString:@""] == NO) {
				symbolDynamicData.bidPrice = [NSNumber numberWithDouble:[bidPrice doubleValue]];
			}
		}
		
		// ask price
		if ([values count] > ASK_PRICE) {
			NSString *askPrice = [values objectAtIndex:ASK_PRICE];
			if ([askPrice isEqualToString:@""] == NO) {
				symbolDynamicData.askPrice = [NSNumber numberWithDouble:[askPrice doubleValue]];
			}
		}
		
		// ask volume
		if ([values count] > ASK_VOLUME) {
			NSString *askVolume = [values objectAtIndex:ASK_VOLUME];
			if ([askVolume isEqualToString:@""] == NO) {
				symbolDynamicData.askVolume = [NSNumber numberWithInteger:[askVolume integerValue]];
			}
		}
		
		// bid volume
		if ([values count] > BID_VOLUME) {
			NSString *bidVolume = [values objectAtIndex:BID_VOLUME];
			if ([bidVolume isEqualToString:@""] == NO) {
				symbolDynamicData.bidVolume = [NSNumber numberWithInteger:[bidVolume integerValue]];
			}
		}
		
		// change
		if ([values count] > CHANGE) {
			NSString *change = [values objectAtIndex:CHANGE];
			if ([change isEqualToString:@""] == NO) {
				symbolDynamicData.lastTradeChange = [NSNumber numberWithDouble:[change doubleValue]];
			}		}
		
		// high
		if ([values count] > HIGH) {
			NSString *high = [values objectAtIndex:HIGH];
			if ([high isEqualToString:@""] == NO) {
				symbolDynamicData.high = [NSNumber numberWithDouble:[high doubleValue]];
			}
		}
		
		// low
		if ([values count] > LOW) {
			NSString *low = [values objectAtIndex:LOW];
			if ([low isEqualToString:@""] == NO) {
				symbolDynamicData.low = [NSNumber numberWithDouble:[low doubleValue]];
			}
		}
		
		// open
		if ([values count] > OPEN) {
			NSString *open = [values objectAtIndex:OPEN];
			if ([open isEqualToString:@""] == NO) {
				symbolDynamicData.open = [NSNumber numberWithDouble:[open doubleValue]];
			}
		}
		
		// volume
		if ([values count] > VOLUME) {
			NSString *volume = [values objectAtIndex:VOLUME];
			if ([volume isEqualToString:@""] == NO) {
				symbolDynamicData.volume = [NSNumber numberWithInteger:[volume integerValue]];
			}
		}
		array = nil;
	}
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		NSLog(@"Whoops.");
	}
}

#pragma mark -
#pragma mark UI Actions
- (void)add:(id)sender {
	SymbolAddController *controller = [[SymbolAddController alloc] initWithNibName:@"SymbolAddView" bundle:nil];

	controller.delegate = self;
	controller.managedObjectContext = self.managedObjectContext;
	controller.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
	[self presentModalViewController:controller animated:YES];
	
	[controller release];
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
	
    if (fetchedResultsController != nil) {
        return fetchedResultsController;
    }
    
	// Create and configure a fetch request with the Book entity.
	NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"SymbolDynamicData" inManagedObjectContext:self.managedObjectContext];
	[fetchRequest setEntity:entity];
	
	// Create the sort descriptors array.
	NSSortDescriptor *mCodeDescriptor = [[NSSortDescriptor alloc] initWithKey:@"symbol.feed.feedName" ascending:YES];
	//NSSortDescriptor *feedDescriptor = [[NSSortDescriptor alloc] initWithKey:@"symbol.feed.description" ascending:YES];
	//NSSortDescriptor *tickerDescriptor = [[NSSortDescriptor alloc] initWithKey:@"symbol.tickerSymbol" ascending:YES];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:mCodeDescriptor, nil];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:@"symbol.feed.feedName" cacheName:@"Root"];
	self.fetchedResultsController = aFetchedResultsController;
	fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[mCodeDescriptor release];
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

- (Feed *)fetchFeed:(NSString *)mCode {
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(mCode=%@)", mCode];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"mCode" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

- (Feed *)fetchFeedByName:(NSString *)feedName {
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feedName=%@)", feedName];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"feedName" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}	
}

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode {
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed.mCode=%@) AND (tickerSymbol=%@)", mCode, tickerSymbol];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}



#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
    NSLog(@"Queried about %@", NSStringFromSelector(sel));
    return [super respondsToSelector:sel];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [super dealloc];
}

@end
