//
//  OrderBookController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "OrderBookController.h"

#import "OrderBookTableCellP.h"
#import "mTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "BidAsk.h"

#import "DataController.h"

@implementation OrderBookController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize symbol = _symbol;
@synthesize bidAsks = _bidAsks;
@synthesize orderbookAvailableLabel = _orderbookAvailableLabel;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		_managedObjectContext = [managedObjectContext retain];
		_tableFont = [[UIFont systemFontOfSize:17.0] retain];
		
		_symbol = nil;
		_bidAsks = nil;
	}
	return self;
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	NSString *labelString = NSLocalizedString(@"noOrderbookAvailable", @"No Orderbook Available");
	UIFont *labelFont = [UIFont boldSystemFontOfSize:17.0f];
	CGRect frame = self.view.bounds;
	frame.size.height = [labelString sizeWithFont:labelFont].height;
	
	_orderbookAvailableLabel = [[UILabel alloc] initWithFrame:frame];
	self.orderbookAvailableLabel.textAlignment = UITextAlignmentCenter;
	self.orderbookAvailableLabel.font = labelFont;
	self.orderbookAvailableLabel.textColor = [UIColor blackColor];
	self.orderbookAvailableLabel.backgroundColor = [UIColor clearColor];
	self.orderbookAvailableLabel.text = labelString;
	self.orderbookAvailableLabel.hidden = YES;
	[self.tableView addSubview:_orderbookAvailableLabel];
	
	[DataController sharedManager].orderBookDelegate = self;
}

#pragma mark -
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	NSUInteger rowCount = [self.bidAsks count];
	
	if (rowCount == 0) {
		self.orderbookAvailableLabel.hidden = NO;
	} else {
		self.orderbookAvailableLabel.hidden = YES;
	}	
	return [self.bidAsks count];
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
	
	NSArray *bidsAsks = [DataController fetchBidAsksForSymbol:self.symbol.tickerSymbol withFeedNumber:self.symbol.feed.feedNumber inManagedObjectContext:self.managedObjectContext];
	self.bidAsks = bidsAsks;
	
	[self.tableView reloadData];
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
	
	[_managedObjectContext release];
    [super dealloc];
}

@end

