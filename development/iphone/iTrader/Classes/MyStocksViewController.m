//
//  MyStocksViewController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "MyStocksViewController.h"
#import "iTraderAppDelegate.h"
#import "iTraderCommunicator.h"
#import "SymbolsController.h"
#import "StockListingCell.h"
#import "Symbol.h"
#import "Feed.h";

@implementation MyStocksViewController

- (id)init {
	self = [super init];
	if (self != nil) {
		self.title = @"My Stocks";
		UIImage* anImage = [UIImage imageNamed:@"infront.png"];
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"MyStocksTab", @"My Stocks tab label") image:anImage tag:MYSTOCKS];
		self.tabBarItem = theItem;
		[theItem release];
		
		symbolsController = [SymbolsController sharedManager];
		communicator = [iTraderCommunicator sharedManager];
		
		symbolsController.updateDelegate = self;

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
	// If logged in move along, but if not and username and password are defined. Log in.
	if (!communicator.isLoggedIn) {
		[communicator login];
	}
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
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

/* Section Handling */

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	//NSLog(@"%@", symbolsController.feeds.count);
	return [symbolsController.orderedFeeds count];
}

//- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView {
//	return [NSArray arrayWithArray:symbolsController.orderedFeeds];
//}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {	
	return [symbolsController.orderedSymbols count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	Feed *feed = [symbolsController.orderedFeeds objectAtIndex:section];
	return feed.feedDescription;
}

/* Row Handling */

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
	
	[cell.contentView setBackgroundColor:[UIColor yellowColor]];
	Symbol *symbol = [symbolsController.orderedSymbols objectAtIndex:indexPath.row];
	//[UIView beginAnimations:nil context:NULL];
//	[UIView setAnimationDuration:0.5];
//	[cell.contentView setBackgroundColor:[UIColor whiteColor]];
//	[UIView commitAnimations];
	
	cell.tickerLabel.text = symbol.ticker;
	cell.nameLabel.text = symbol.name;
	[cell.valueButton setTitle:[symbol.lastTrade stringValue] forState:UIControlStateNormal];
	
	return cell;
}

- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
	return YES;
}

/**
 * This method should receive a list of symbols that have been updated and should
 * update any rows necessary.
 */
- (void)symbolsUpdated:(NSArray *)feedTickers {
	NSArray *indexPaths = [[NSMutableArray alloc] init];
	for (NSString *feedTicker in feedTickers) {
		NSArray *feedTickerTuple = [feedTicker componentsSeparatedByString:@"/"];		
		
		NSString *feed = [feedTickerTuple objectAtIndex:0];
				
		NSInteger section = [[symbolsController.feeds valueForKey:feed] integerValue];
		NSInteger row = [[symbolsController.symbols valueForKey:feedTicker] integerValue];
		NSIndexPath *itemToUpdate = [[NSIndexPath alloc] init];
		itemToUpdate.section = section;
		itemToUpdate.row = row;
		[indexPaths addObject:itemToUpdate];
		[indexPath release];
	}
		
	[self.tableView reloadRowsAtIndexPaths:indexPaths withRowAnimation:UITableViewRowAnimationFade];
	[indexPaths release];
}

@end
