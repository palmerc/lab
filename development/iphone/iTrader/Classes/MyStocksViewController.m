//
//  MyStocksViewController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "MyStocksViewController.h"

#import "iTraderAppDelegate.h"

#import "SymbolsController.h"
#import "Symbol.h"
#import "Feed.h";

#import "StockDetailController.h"
#import "StockSearchController.h"
#import "StockListingCell.h"

@implementation MyStocksViewController
@synthesize symbolsController = _symbolsController;

- (id)init {
	self = [super init];
	if (self != nil) {
		self.title = @"My Stocks";
		UIImage* anImage = [UIImage imageNamed:@"infront.png"];
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"MyStocksTab", @"My Stocks tab label") image:anImage tag:MYSTOCKS];
		self.tabBarItem = theItem;
		[theItem release];
		
		_symbolsController = [SymbolsController sharedManager];
		_communicator = [iTraderCommunicator sharedManager];
		
		_symbolsController.updateDelegate = self;	
	}
	return self;
}

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(addStock:)];
	self.navigationItem.rightBarButtonItem = addItem;
	[addItem release];
	[_communicator login];
	
	/*
	UIButton *editStocksButton = [UIButton buttonWithType:UIButtonTypeInfoLight];
	CGRect *buttonRect = editStocksButton.frame;
	buttonRect.origin.x = self.frame.size.width - buttonRect.size.width - 8;
	buttonRect.origin.y = self.frame.size.height - buttonRect.size.height - 8;
	[editStocksButton setFrame:buttonRect];
	[editStocksButton addTarget:self action:@selector(editStocks:) forControlEvents:UIControlEventTouchUpInside];
	[editStocksButton setEnabled:YES];
	[self.view addSubview:editStocksButton];
	 */
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait | UIInterfaceOrientationLandscapeLeft);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

/**
 * tableView Delegation
 *
 */


/* Section Handling */

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [self.symbolsController.feeds count];
}

//- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView {
//	return [NSArray arrayWithArray:symbolsController.orderedFeeds];
//}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {	
	return [[[self.symbolsController.feeds objectAtIndex:section] symbols] count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	Feed *feed = [self.symbolsController.feeds objectAtIndex:section];
	return feed.feedDescription;
}

/* Row Handling */

/**
 * This is called everytime the table wants the data for a specific row in the visible table.
 * The table view only ever asks for the visible cells.
 */
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	static NSString *CellIdentifier = @"SymbolCell";
	
	StockListingCell *cell = (StockListingCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	if (cell == nil) {
		NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:@"StockListingCell" owner:nil options:nil];
		for (id currentObject in topLevelObjects) {
			if ([currentObject isKindOfClass:[StockListingCell class]]) {
				cell = (StockListingCell *)currentObject;
				break;
			}
		}
	}
		
	Symbol *symbol = [self.symbolsController symbolAtIndexPath:indexPath];
	//NSLog(@"change: %@", symbol.changeSinceLastUpdate);
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
	cell.editing = YES;
	cell.tickerLabel.text = symbol.tickerSymbol;
	cell.nameLabel.text = symbol.name;
	[cell.valueButton setTitle:[symbol.lastTrade stringValue] forState:UIControlStateNormal];
	
	return cell;
}

// This method is required to catch the swipe to delete gesture.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
	if (editingStyle == UITableViewCellEditingStyleDelete) {
		NSLog(@"Delete %@", indexPath);
	}

}

- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
	return YES;
}

- (NSIndexPath *)tableView:(UITableView *)tableView:(NSIndexPath *)sourceIndexPath toProposedIndexPath:(NSIndexPath *)proposedDestinationIndexPath {
	return [NSIndexPath indexPathForRow:0 inSection:0];
}

- (BOOL)tableView:(UITableView *)tableView shouldIndentWhileEditingRowAtIndexPath:(NSIndexPath *)indexPath {
	return YES;
}

- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	// TODO Make this work for more than simply the first stock
	Symbol *symbol = [self.symbolsController symbolAtIndexPath:indexPath];
	StockDetailController *detailController = [[StockDetailController alloc] initWithSymbol:symbol];
	[self.navigationController pushViewController:detailController animated:YES];
	
	[detailController release];
}



/**
 * Delegation
 */


- (void)symbolDeleted {

}

- (void)feedAdded:(Feed *)feed {
	[self.tableView reloadData];
}

// Additions and Updates
- (void)symbolsAdded:(NSArray *)symbols {
	NSMutableArray *indexPaths = [[NSMutableArray alloc] init];
	
	for (Symbol *symbol in symbols) {
		NSIndexPath *indexPath = [self.symbolsController indexPathOfSymbol:symbol.feedTicker];
		[indexPaths addObject:indexPath];
	}
	
	[self.tableView beginUpdates];
	[self.tableView insertRowsAtIndexPaths:indexPaths withRowAnimation:UITableViewRowAnimationNone];
	[self.tableView endUpdates];
	
	[indexPaths release];
}

/**
 * This method should receive a list of symbols that have been updated and should
 * update any rows necessary.
 */
- (void)symbolsUpdated:(NSArray *)feedTickers {
	NSMutableArray *indexPaths = [[NSMutableArray alloc] init];

	[self.tableView beginUpdates];
	for (NSString *feedTicker in feedTickers) {
		NSIndexPath *indexPath = [self.symbolsController indexPathOfSymbol:feedTicker];
		[indexPaths addObject:indexPath];
	}
	
	[self.tableView reloadRowsAtIndexPaths:indexPaths withRowAnimation:UITableViewRowAnimationNone];
	[self.tableView endUpdates];
	[indexPaths release];
}

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

@end
