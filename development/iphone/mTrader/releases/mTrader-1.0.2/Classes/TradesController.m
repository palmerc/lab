//
//  TradesController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesController.h"

#import "SymbolDataController.h"

#import <QuartzCore/QuartzCore.h>
#import "mTraderCommunicator.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Feed.h"
#import "Trade.h"

#import "TradesCell.h"

@implementation TradesController
@synthesize delegate;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize symbol = _symbol;
@synthesize trades = _trades;

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
	[super viewDidLoad];

	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
	table = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
	[self.view addSubview:table];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
	
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
}

- (void)viewDidAppear:(BOOL)animated {
	[super viewDidAppear:animated];

	CGRect viewFrame = self.view.bounds;
	
	CGFloat width = 320.0 / 3;
	CGSize textSize = [@"X" sizeWithFont:[UIFont boldSystemFontOfSize:18.0]];
	CGFloat y = viewFrame.origin.y;
	CGRect frame = CGRectMake(0.0, y, width, textSize.height);
	tradeTimeLabel = [[self setHeader:@"Time" withFrame:frame] retain];
	frame = CGRectMake(width, y, width, textSize.height);
	tradePriceLabel = [[self setHeader:@"Price" withFrame:frame] retain];
	frame = CGRectMake(width * 2, y, width, textSize.height);
	tradeVolumeLabel = [[self setHeader:@"Size" withFrame:frame] retain];
	
	viewFrame.origin.y += textSize.height;
	viewFrame.size.height -= textSize.height;
	
	table.frame = viewFrame;
	table.delegate = self;
	table.dataSource = self;
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] tradesRequest:feedTicker];
	
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

#pragma mark -
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [self.trades count];
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"TradesCell";
    
    TradesCell *cell = (TradesCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[TradesCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		//cell.mainFont = [UIFont systemFontOfSize:17.0];
		//cell.size = CGSizeMake(self.table.frame.size.width, self.table.frame.size.height);
	}
    
    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(TradesCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	Trade *trade =  [self.trades objectAtIndex:indexPath.row];
	
	cell.trade = trade;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    CGSize size = [@"X" sizeWithFont:[UIFont systemFontOfSize:17.0]];
	return size.height;
}

- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	[self.symbol addObserver:self forKeyPath:@"trades" options:NSKeyValueObservingOptionNew context:nil];
	[self updateTrades];
}

- (void)updateTrades {
	NSArray *trades = [SymbolDataController fetchTradesForSymbol:self.symbol.tickerSymbol withFeedNumber:self.symbol.feed.feedNumber inManagedObjectContext:self.managedObjectContext];
	self.trades = trades;
	
	[table reloadData];
}


#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"trades"]) {
		[self updateTrades];
	}
}

- (void)done:(id)sender {
	[self.delegate tradesControllerDidFinish:self];
}

- (void)refresh:(id)sender {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[communicator tradesRequest:feedTicker];
}

- (void)dealloc {
	[self.symbol removeObserver:self forKeyPath:@"trades"];	

	[_managedObjectContext release];
	[_symbol release];
	[_trades release];
	
	[tradeTimeLabel release];
	[tradePriceLabel release];
	[tradeVolumeLabel release];
	
	[table release];
	
    [super dealloc];
}


@end

