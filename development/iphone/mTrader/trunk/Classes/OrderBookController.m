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
#import "BidAsk.h"

#import "SymbolDataController.h"

@implementation OrderBookController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize symbol = _symbol;
@synthesize bidAsks = _bidAsks;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		_tableFont = [[UIFont systemFontOfSize:17.0] retain];
		
		_symbol = nil;
		_bidAsks = nil;
	}
	return self;
}

- (void)viewDidLoad {
	[SymbolDataController sharedManager].orderBookDelegate = self;
}

#pragma mark -
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [self.bidAsks count];
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"OrderBookTableCellP";
    
    OrderBookTableCellP *cell = (OrderBookTableCellP *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[OrderBookTableCellP alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		cell.mainFont = _tableFont;
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
	}
	cell.maxWidth = self.view.frame.size.width;

    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(OrderBookTableCellP *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	BidAsk *bidAsk = [self.bidAsks objectAtIndex:indexPath.row];
	
	cell.bidAsk = bidAsk;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    CGSize size = [@"X" sizeWithFont:_tableFont];
	return size.height;
}

- (void)updateOrderBook {
	if (_bidAsks != nil) {
		[_bidAsks release];
		_bidAsks = nil;
	}
	
	NSArray *bidsAsks = [SymbolDataController fetchBidAsksForSymbol:self.symbol.tickerSymbol withFeedNumber:self.symbol.feed.feedNumber inManagedObjectContext:self.managedObjectContext];
	self.bidAsks = bidsAsks;
	
	[self.tableView reloadData];
}

//#pragma mark -
//#pragma mark Debugging methods
// // Very helpful debug when things seem not to be working.
// - (BOOL)respondsToSelector:(SEL)sel {
//	 NSLog(@"Queried about %@ in OrderBookController", NSStringFromSelector(sel));
//	 return [super respondsToSelector:sel];
// }

#pragma mark -
#pragma mark Memory management
- (void)dealloc {	
	[_tableFont release];
	[_symbol release];
	[_bidAsks release];
	
	[_managedObjectContext release];
    [super dealloc];
}

@end

