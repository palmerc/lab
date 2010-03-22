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
@synthesize delegate;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize symbol = _symbol;
@synthesize bidAsks = _bidAsks;
@synthesize table;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		
		tableFont = nil;
	}
	return self;
}

- (void)loadView {
	UIView *aView = [[UIView alloc] init];
		
	tableFont = [[UIFont systemFontOfSize:17.0] retain];
	table = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
	table.delegate = self;
	table.dataSource = self;
	
	[aView addSubview:table];
	
	self.view = aView;
	[aView release];
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
    static NSString *CellIdentifier = @"ChainsTableCell";
    
    OrderBookTableCellP *cell = (OrderBookTableCellP *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[OrderBookTableCellP alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		cell.mainFont = [UIFont systemFontOfSize:17.0];
		cell.size = CGSizeMake(self.table.frame.size.width, self.table.frame.size.height);
	}
    
    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(OrderBookTableCellP *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	BidAsk *bidAsk = [self.bidAsks objectAtIndex:indexPath.row];
	
	cell.bidAsk = bidAsk;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    CGSize size = [@"X" sizeWithFont:tableFont];
	return size.height;
}

- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	[self.symbol addObserver:self forKeyPath:@"bidsAsks" options:NSKeyValueObservingOptionNew context:nil];
	[self updateSymbol];
}

- (void)updateSymbol {
	NSArray *bidsAsks = [SymbolDataController fetchBidAsksForSymbol:self.symbol.tickerSymbol withFeed:self.symbol.feed.mCode inManagedObjectContext:self.managedObjectContext];
	self.bidAsks = bidsAsks;
	
	[self.table reloadData];
}


#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"bidsAsks"]) {
		[self updateSymbol];
	}
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
	[self.symbol removeObserver:self forKeyPath:@"bidsAsks"];
	
	[tableFont release];
	[_symbol release];
	[_managedObjectContext release];
	[table release];
    [super dealloc];
}

@end

