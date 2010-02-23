//
//  OrderBookController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "OrderBookController.h"

#import "OrderBookTableCellP.h"
#import "mTraderCommunicator.h"
#import "StringHelpers.h"
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Ask.h"
#import "Bid.h"

@implementation OrderBookController
@synthesize managedObjectContext;
@synthesize asks, bids;
@synthesize symbol = _symbol;

- (id)initWithSymbol:(Symbol *)symbol {
	self = [super init];
	if (self != nil) {
		self.symbol = symbol;
		self.managedObjectContext = [symbol managedObjectContext];
		
		asks = [[NSMutableArray alloc] init];
		bids = [[NSMutableArray alloc] init];
	}
	return self;
}

/*
- (id)initWithStyle:(UITableViewStyle)style {
    // Override initWithStyle: if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
    if (self = [super initWithStyle:style]) {
    }
    return self;
}
*/

- (void)viewDidLoad {
    [super viewDidLoad];
	
	[[mTraderCommunicator sharedManager] stopStreamingData];
	[mTraderCommunicator sharedManager].symbolsDelegate = self;
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] orderBookForFeedTicker:feedTicker];
}


/*
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}
*/
/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/
/*
- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
}
*/
/*
- (void)viewDidDisappear:(BOOL)animated {
	[super viewDidDisappear:animated];
}
*/

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


#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [asks count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	return nil;	
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"OrderBookCell";
    
    OrderBookTableCellP *cell = (OrderBookTableCellP *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[OrderBookTableCellP alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}


- (void)configureCell:(OrderBookTableCellP *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	NSInteger row = indexPath.row;
	Ask *ask = [asks objectAtIndex:row];
	Bid *bid = [bids objectAtIndex:row];

	cell.bid = bid;
	cell.ask = ask;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    // Navigation logic may go here. Create and push another view controller.
	// AnotherViewController *anotherViewController = [[AnotherViewController alloc] initWithNibName:@"AnotherView" bundle:nil];
	// [self.navigationController pushViewController:anotherViewController];
	// [anotherViewController release];
}


/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/


/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/


/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/


/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

/**
 * This method should receive a list of symbols that have been updated and should
 * update any rows necessary.
 */
- (void)updateSymbols:(NSArray *)updates {
	// l;cp;v;d
	static NSInteger FEED_TICKER = 0;
	static NSInteger LAST_TRADE = 1;
	static NSInteger PERCENT_CHANGE = 2;
	static NSInteger VOLUME = 3;
	static NSInteger ORDERBOOK = 4;
	
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
		
		// last trade
		if ([values count] > LAST_TRADE) {
			NSString *lastTrade = [values objectAtIndex:LAST_TRADE];
			if ([lastTrade isEqualToString:@"--"] == YES || [lastTrade isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.lastTrade = nil;
			} else if ([lastTrade isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.lastTrade = [NSNumber numberWithDouble:[lastTrade doubleValue]];
				symbol.symbolDynamicData.lastTradeTime = [NSDate date];
			}
		}
		
		// percent change
		if ([values count] > PERCENT_CHANGE) {
			NSString *percentChange = [values objectAtIndex:PERCENT_CHANGE];
			if ([percentChange isEqualToString:@"--"] == YES || [percentChange isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.changePercent = nil;
			} else if ([percentChange isEqualToString:@""] == NO) {
				symbol.symbolDynamicData.changePercent = [NSNumber numberWithDouble:([percentChange doubleValue]/100.0)];
			}
		}
		
		// volume
		if ([values count] > VOLUME) {
			NSString *volume = [values objectAtIndex:VOLUME];
			if ([volume isEqualToString:@"--"] == YES || [volume isEqualToString:@"-"] == YES) {
				symbol.symbolDynamicData.volume = nil;
			} else if ([volume isEqualToString:@""] == NO) {
				float multiplier = 1.0;
				if ([volume rangeOfString:@"k"].location != NSNotFound) {
					multiplier = 1000.0;
				} else if ([volume rangeOfString:@"m"].location != NSNotFound) {
					multiplier = 1000000.0;
				}
				symbol.symbolDynamicData.volume = [NSNumber numberWithDouble:[volume doubleValue]  * multiplier];
			}
		}
		
		// orderBook
		if ([values count] > ORDERBOOK) {
			// Bid then Ask
			
			NSArray *orderBook = [[values objectAtIndex:ORDERBOOK] componentsSeparatedByString:@"#"];
			orderBook = [StringHelpers cleanComponents:orderBook];
			NSInteger half = [orderBook count] / 2;
			for (int i = 0; i < half; i++) {
				NSString *orderBookString = [orderBook objectAtIndex:i];
				if (![orderBookString isEqualToString:@""]) {
					if ([bids count] > i && ![orderBookString isEqualToString:@""]) {
						[bids removeObjectAtIndex:i];
					}
					NSArray *pieces = [orderBookString componentsSeparatedByString:@"\\"];
					Bid *bid = [[Bid alloc] init];
					NSString *value = [pieces objectAtIndex:0];
					NSString *size = [pieces objectAtIndex:1];
					
					float multiplier = 1.0;
					if ([size rangeOfString:@"k"].location != NSNotFound) {
						multiplier = 1000.0;
					} else if ([size rangeOfString:@"m"].location != NSNotFound) {
						multiplier = 1000000.0;
					}
					
					bid.bidValue = [NSNumber numberWithDouble:[value doubleValue]];
					bid.bidSize = [NSNumber numberWithInteger:[size integerValue] * multiplier];
					[bids insertObject:bid atIndex:i];
					[bid release];
				}
			}
			
			for (int i = 0; i < half; i++) {
				NSString *orderBookString = [orderBook objectAtIndex:i + half];
				if (![orderBookString isEqualToString:@""] && ![orderBookString isEqualToString:@"/"]) {
					if ([asks count] > i) {
						[asks removeObjectAtIndex:i];
					}
					
					if ([orderBookString rangeOfString:@"/"].location == 0) {
						orderBookString = [orderBookString substringFromIndex:1];
					}
					NSArray *pieces = [orderBookString componentsSeparatedByString:@"\\"];
					Ask *ask = [[Ask alloc] init];
					NSString *value = [pieces objectAtIndex:0];
					NSString *size = [pieces objectAtIndex:1];
					
					float multiplier = 1.0;
					if ([size rangeOfString:@"k"].location != NSNotFound) {
						multiplier = 1000.0;
					} else if ([size rangeOfString:@"m"].location != NSNotFound) {
						multiplier = 1000000.0;
					}
					
					ask.askValue = [NSNumber numberWithDouble:[value doubleValue]];
					ask.askSize = [NSNumber numberWithInteger:[size integerValue] * multiplier];
					[asks insertObject:ask atIndex:i];
					[ask release];
				}
			}
			
			NSSortDescriptor *sortDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"askSize" ascending:NO] autorelease];
			NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
			NSArray *sortedAsks = [asks sortedArrayUsingDescriptors:sortDescriptors];
			
			sortDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"bidSize" ascending:NO] autorelease];
			sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
			NSArray *sortedBids = [bids sortedArrayUsingDescriptors:sortDescriptors];
					
			Ask *biggestAskSize = [sortedAsks objectAtIndex:0];
			Bid *biggestBidSize = [sortedBids objectAtIndex:0];
			
			for (Ask *ask in asks) {
				ask.percent = [ask.askSize floatValue] / [biggestAskSize.askSize floatValue];
			}
			
			for (Bid *bid in bids) {
				bid.percent = [bid.bidSize floatValue] / [biggestBidSize.bidSize floatValue];
			}
		}
		array = nil;
	}
	
	[self.tableView reloadData];
}

- (void)dealloc {
	[self.symbol release];
	[self.managedObjectContext release];
	
    [super dealloc];
}


@end

