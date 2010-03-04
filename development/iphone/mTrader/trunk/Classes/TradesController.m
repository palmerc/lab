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
#import "Feed.h"
#import "Trade.h"

#import "TradesCell.h"

@implementation TradesController
@synthesize delegate;
@synthesize symbol = _symbol;
@synthesize trades = _trades;

- (id)initWithSymbol:(Symbol *)symbol {
	self = [super init];
	if (self != nil) {
		_symbol = symbol;
		_trades = nil;
	}
	return self;
}

- (void)viewDidLoad {
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
	table = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
	[self.view addSubview:table];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
	
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
	
	[super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
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
	
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	[communicator stopStreamingData];
	communicator.symbolsDelegate = self;
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[communicator tradesRequest:feedTicker];
	
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

#pragma mark Tableview methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [self.trades count];
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"TradesCell";
    
    TradesCell *cell = (TradesCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[TradesCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(TradesCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	NSInteger row = indexPath.row;
	Trade *t = [self.trades objectAtIndex:row];
	
	cell.trade = t;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    CGSize size = [@"X" sizeWithFont:[UIFont systemFontOfSize:17.0]];
	return size.height;
}

- (void)tradesUpdate:(NSDictionary *)updateDictionary {
	NSString *tradesString = [updateDictionary objectForKey:@"trades"];
	NSArray *tradesComponents = [tradesString componentsSeparatedByString:@"|"];
	NSMutableArray *tradesTemporaryStorage = [[NSMutableArray alloc] init];
	
	for (NSString *trade in tradesComponents) {
		NSArray *parts = [trade componentsSeparatedByString:@";"];
		Trade *t = [[Trade alloc] init];
		NSString *time = [parts objectAtIndex:0];
		NSString *price = [parts objectAtIndex:1];
		NSString *volume = [parts objectAtIndex:2];
		
		t.time = time;
		t.price = [NSNumber numberWithDouble:[price doubleValue]];
		t.volume = [NSNumber numberWithInteger:[volume integerValue]];
		
		[tradesTemporaryStorage addObject:t];
		[t release];
	}
		
	self.trades = tradesTemporaryStorage;
	[tradesTemporaryStorage release];
	
	[table reloadData];
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
	[tradeTimeLabel release];
	[tradePriceLabel release];
	[tradeVolumeLabel release];
	[_trades release];
    [super dealloc];
}


@end

