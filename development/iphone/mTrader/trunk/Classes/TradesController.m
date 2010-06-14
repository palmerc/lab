//
//  TradesController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesController.h"

#import <QuartzCore/QuartzCore.h>
#import "mTraderCommunicator.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Feed.h"
#import "Trade.h"

#import "TradesCell.h"

@implementation TradesController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize symbol = _symbol;
@synthesize trades = _trades;
@synthesize tradesAvailableLabel = _tradesAvailableLabel;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		_symbol = nil;
		_trades = nil;
	}
	return self;
}

- (void)viewDidLoad {
	NSString *labelString = @"No Trades Data Available";
	UIFont *labelFont = [UIFont boldSystemFontOfSize:24.0f];
	CGRect frame = self.view.bounds;
	frame.size.height = [labelString sizeWithFont:labelFont].height;
	
	_tradesAvailableLabel = [[UILabel alloc] initWithFrame:frame];
	self.tradesAvailableLabel.textAlignment = UITextAlignmentCenter;
	self.tradesAvailableLabel.font = labelFont;
	self.tradesAvailableLabel.textColor = [UIColor blackColor];
	self.tradesAvailableLabel.backgroundColor = [UIColor clearColor];
	self.tradesAvailableLabel.text = labelString;
	self.tradesAvailableLabel.hidden = YES;
	[self.tableView addSubview:self.tradesAvailableLabel];
	
	[DataController sharedManager].tradesDelegate = self;
}

- (void)viewDidUnload {
	id tradesDelegate = [DataController sharedManager].tradesDelegate;
	if (tradesDelegate == self) {
		tradesDelegate = nil;
	}
}

#pragma mark -
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	NSUInteger rowCount = [self.trades count];

	if (rowCount == 0) {
		self.tradesAvailableLabel.hidden = NO;
	} else {
		self.tradesAvailableLabel.hidden = YES;
	}
	
	return rowCount;
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"TradesCell";
    
    TradesCell *cell = (TradesCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[TradesCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
	}
    
    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(TradesCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	Trade *trade =  [self.trades objectAtIndex:indexPath.row];
	
	cell.trade = trade;
}

#pragma mark -
#pragma mark UITableViewDelegate methods
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	UIFont *mainFont = [UIFont systemFontOfSize:14.0f];
	UIFont *timeFont = [UIFont systemFontOfSize:12.0f];
	CGSize mainSize = [@"X" sizeWithFont:mainFont];
	CGSize timeSize = [@"X" sizeWithFont:timeFont];
	
	return mainSize.height + timeSize.height;
}

- (void)setSymbol:(Symbol *)symbol {
	if (_symbol != nil) {
		[_symbol release];
	}
	_symbol = [symbol retain];
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] tradesRequest:feedTicker];
}

- (void)updateTrades {
	NSArray *trades = [DataController fetchTradesForSymbol:self.symbol.tickerSymbol withFeedNumber:self.symbol.feed.feedNumber inManagedObjectContext:self.managedObjectContext];
	self.trades = trades;
	
	[self.tableView reloadData];
}

- (void)refresh:(id)sender {
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] tradesRequest:feedTicker];
}

- (void)dealloc {
	[_managedObjectContext release];
	[_symbol release];
	[_trades release];
	[_tradesAvailableLabel release];

    [super dealloc];
}


@end

