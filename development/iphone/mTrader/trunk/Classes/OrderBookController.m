//
//  OrderBookController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "OrderBookController.h"

#import "OrderBookView.h"
#import "OrderBookTableCellP.h"
#import "mTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "BidAsk.h"

#import "DataController.h"

@interface OrderBookController ()
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end

@implementation OrderBookController
@synthesize managedObjectContext = _managedObjectContext;

- (id)initWithSymbol:(Symbol *)symbol {
	self = [super init];
	if (self != nil) {
		_symbol = [symbol retain];
		
		_tableView = nil;
		_orderbookAvailableLabel = nil;
		_managedObjectContext = nil;		
		_bidAsks = nil;
	}
	return self;
}

- (void)loadView {
	_tableFont = [UIFont systemFontOfSize:14.0f];
	OrderBookView *orderBookView = [[OrderBookView alloc] initWithFrame:CGRectZero];
	orderBookView.tableView.delegate = self;
	orderBookView.tableView.dataSource = self;
	_tableView = [orderBookView.tableView retain];
	_orderbookAvailableLabel = [orderBookView.orderbookAvailableLabel retain];
	
	self.view = orderBookView;
	[orderBookView release];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	[DataController sharedManager].orderBookDelegate = self;
}

#pragma mark -
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	NSUInteger rowCount = [_bidAsks count];
	
	if (rowCount == 0) {
		_orderbookAvailableLabel.hidden = NO;
	} else {
		_orderbookAvailableLabel.hidden = YES;
	}	
	return [_bidAsks count];
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"OrderBookTableCell_Phone";
    
    OrderBookTableCellP *cell = (OrderBookTableCellP *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[OrderBookTableCellP alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		cell.mainFont = _tableFont;
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
	}

    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(OrderBookTableCellP *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	BidAsk *bidAsk = [_bidAsks objectAtIndex:indexPath.row];
	
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
	
	NSArray *bidsAsks = [DataController fetchBidAsksForSymbol:_symbol.tickerSymbol withFeedNumber:_symbol.feed.feedNumber inManagedObjectContext:_managedObjectContext];
	[_bidAsks release];
	_bidAsks = [bidsAsks retain];
	
	[_tableView reloadData];
}

#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
#if DEBUG
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in OrderBookController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[DataController sharedManager].orderBookDelegate = nil;	
	
	[_tableFont release];
	[_symbol release];
	[_bidAsks release];
	[_orderbookAvailableLabel release];
	[_tableView release];	
	
	[_managedObjectContext release];
    [super dealloc];
}

@end

