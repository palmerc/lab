//
//  MyStocksViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "MyStocksViewController.h"

#import "mTraderAppDelegate.h"

#import "SymbolsController.h"
#import "Symbol.h"
#import "Feed.h";

#import "StockDetailController.h"
#import "StockSearchController.h"
#import "StockListingCell.h"

@implementation MyStocksViewController
@synthesize symbolsController = _symbolsController;
@synthesize editing = _editing;

#pragma mark -
#pragma mark Lifecycle

- (id)init {
	self = [super init];
	if (self != nil) {
		self.title = NSLocalizedString(@"MyStocksTab", @"My Stocks tab label");
		UIImage* anImage = [UIImage imageNamed:@"myStocksTabButton.png"];
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"MyStocksTab", @"My Stocks tab label") image:anImage tag:MYSTOCKS];
		self.tabBarItem = theItem;
		[theItem release];
				
		_symbolsController = [SymbolsController sharedManager];
		_communicator = [mTraderCommunicator sharedManager];
		
		currentValueType = PRICE;
		self.editing = NO;
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

	self.symbolsController.updateDelegate = self;

	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(addStockButtonWasPressed:)];
	self.navigationItem.rightBarButtonItem = addItem;
	[addItem release];
	
	self.navigationItem.leftBarButtonItem = self.editButtonItem;
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait | UIInterfaceOrientationLandscapeLeft);
	return NO;
}
*/
- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewWillDisappear:(BOOL)animated {
	//self.symbolsController.updateDelegate = nil;
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
	self.symbolsController.updateDelegate = nil;
}


- (void)dealloc {
    [super dealloc];
}

/**
 * tableView Delegation
 *
 */


- (void)setEditing:(BOOL)editing animated:(BOOL)animate {
	if (editing == YES) {
		self.editing = YES;
	} else {
		self.editing = NO;
		[self.tableView reloadData];
	}
	[super setEditing:editing animated:animate];
}

#pragma mark -
#pragma mark Section Handling
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

// This method is required to catch the swipe to delete gesture.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
	if (editingStyle == UITableViewCellEditingStyleDelete) {
		[self.symbolsController removeSymbol:indexPath];
		[self.tableView reloadData];
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
	Symbol *symbol = [self.symbolsController symbolAtIndexPath:indexPath];
	StockDetailController *detailController = [[StockDetailController alloc] initWithSymbol:symbol];
	[self.navigationController pushViewController:detailController animated:YES];
	[detailController release];
}

#pragma mark -
#pragma mark Delegation
/**
 * Delegation
 */


-(void) symbolRemoved:(NSIndexPath *)indexPath {
	[self.tableView beginUpdates];
	[self.tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
	[self.tableView endUpdates];
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

@end
