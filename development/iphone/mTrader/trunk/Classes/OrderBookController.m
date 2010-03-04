//
//  OrderBookController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "OrderBookController.h"

#import <QuartzCore/QuartzCore.h>

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
@synthesize table;
@synthesize delegate;

- (id)initWithSymbol:(Symbol *)symbol {
	self = [super init];
	if (self != nil) {
		self.symbol = symbol;
		self.managedObjectContext = [symbol managedObjectContext];
		
		table = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
		asks = [[NSMutableArray alloc] init];
		bids = [[NSMutableArray alloc] init];

		[self.view addSubview:table];
	}
	return self;
}

- (void)viewDidLoad {
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
	
	[super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
	CGRect viewFrame = self.view.bounds;
	
	CGFloat width = 320.0 / 4;
	CGSize textSize = [@"X" sizeWithFont:[UIFont boldSystemFontOfSize:18.0]];
	CGFloat y = viewFrame.origin.y;
	CGRect frame = CGRectMake(0.0, y, width, textSize.height);
	askSizeLabel = [[self setHeader:@"A Size" withFrame:frame] retain];
	frame = CGRectMake(width, y, width, textSize.height);
	askValueLabel = [[self setHeader:@"A Price" withFrame:frame] retain];
	frame = CGRectMake(width * 2, y, width, textSize.height);
	bidSizeLabel = [[self setHeader:@"B Size" withFrame:frame] retain];
	frame = CGRectMake(width * 3, y, width, textSize.height);
	bidValueLabel = [[self setHeader:@"B Price" withFrame:frame] retain];
	
	viewFrame.origin.y += textSize.height;
	
	table.frame = viewFrame;
	table.delegate = self;
	table.dataSource = self;
	
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	[communicator stopStreamingData];
	communicator.symbolsDelegate = self;
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[communicator orderBookForFeedTicker:feedTicker];
	
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

#define TEXT_LEFT_MARGIN    8.0

- (UIView *)setHeader:(NSString *)header withFrame:(CGRect)frame {
	UIFont *headerFont = [UIFont boldSystemFontOfSize:18.0];

	UIColor *sectionTextColor = [UIColor colorWithWhite:1.0 alpha:1.0];
	UIColor *sectionTextShadowColor = [UIColor colorWithWhite:0.0 alpha:0.44];
	CGSize shadowOffset = CGSizeMake(0.0, 1.0);
	
	// Render the dynamic gradient
	CAGradientLayer *headerGradient = [CAGradientLayer layer];
	UIColor *topLine = [UIColor colorWithRed:111.0/255.0 green:118.0/255.0 blue:123.0/255.0 alpha:1.0];
	UIColor *shine = [UIColor colorWithRed:165.0/255.0 green:177/255.0 blue:186.0/255.0 alpha:1.0];
	UIColor *topOfFade = [UIColor colorWithRed:144.0/255.0 green:159.0/255.0 blue:170.0/255.0 alpha:1.0];
	UIColor *bottomOfFade = [UIColor colorWithRed:184.0/255.0 green:193.0/255.0 blue:200.0/255.0 alpha:1.0];
	UIColor *bottomLine = [UIColor colorWithRed:152.0/255.0 green:158.0/255.0 blue:164.0/255.0 alpha:1.0];
	NSArray *colors = [NSArray arrayWithObjects:(id)topLine.CGColor, (id)shine.CGColor, (id)topOfFade.CGColor, (id)bottomOfFade.CGColor, (id)bottomLine.CGColor, nil];
	NSArray *locations = [NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0], [NSNumber numberWithFloat:0.05],[NSNumber numberWithFloat:0.10],[NSNumber numberWithFloat:0.95],[NSNumber numberWithFloat:1.0],nil];
	headerGradient.colors = colors;
	headerGradient.locations = locations;
	
	CGSize headerSize = [header sizeWithFont:headerFont];
	CGFloat xOffset = (frame.size.width - headerSize.width)/2;
	CGRect labelFrame = CGRectMake(xOffset, 0.0, headerSize.width, headerSize.height);
	
	UIView *headerView = [[[UIView alloc] initWithFrame:frame] autorelease];
	UILabel *label = [[UILabel alloc] initWithFrame:labelFrame];
	
	[headerView.layer insertSublayer:headerGradient atIndex:0];
	headerGradient.frame = headerView.bounds;
	
	label.text = header;
	[label setFont:headerFont];
	[label setTextColor:sectionTextColor];
	[label setShadowColor:sectionTextShadowColor];
	[label setShadowOffset:shadowOffset];
	[label setBackgroundColor:[UIColor clearColor]];
	
	[headerView addSubview:label];
	[self.view addSubview:headerView];
	
	[label release];
	return headerView;
}

#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [asks count];
}

//- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
//	return nil;	
//}

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

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    CGSize size = [@"X" sizeWithFont:[UIFont systemFontOfSize:17.0]];
	return size.height;
}

//- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    // Navigation logic may go here. Create and push another view controller.
	// AnotherViewController *anotherViewController = [[AnotherViewController alloc] initWithNibName:@"AnotherView" bundle:nil];
	// [self.navigationController pushViewController:anotherViewController];
	// [anotherViewController release];
//}


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
			
			if ([sortedAsks count] > 0) {
				Ask *biggestAskSize = [sortedAsks objectAtIndex:0];
				
				for (Ask *ask in asks) {
					ask.percent = [ask.askSize floatValue] / [biggestAskSize.askSize floatValue];
				}
			}
			
			if ([sortedBids count] > 0) {
				Bid *biggestBidSize = [sortedBids objectAtIndex:0];
					
				for (Bid *bid in bids) {
					bid.percent = [bid.bidSize floatValue] / [biggestBidSize.bidSize floatValue];
				}
			}
		}
		array = nil;
	}
	
	[self.table reloadData];
}

- (void)done:(id)sender {
	[[mTraderCommunicator sharedManager] stopStreamingData];
	[mTraderCommunicator sharedManager].symbolsDelegate = nil;

	[self.delegate orderBookControllerDidFinish:self];
}

#pragma mark -
#pragma mark Debugging methods
 // Very helpful debug when things seem not to be working.
 - (BOOL)respondsToSelector:(SEL)sel {
	 NSLog(@"Queried about %@ in OrderBookController", NSStringFromSelector(sel));
	 return [super respondsToSelector:sel];
 }

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[self.asks release];
	[self.bids release];
	[self.symbol release];
	[self.managedObjectContext release];
	[table release];
    [super dealloc];
}

@end

