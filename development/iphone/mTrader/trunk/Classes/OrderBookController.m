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

- (id)init {
	self = [super init];
	if (self != nil) {				
		asks = [[NSMutableArray alloc] init];
		bids = [[NSMutableArray alloc] init];
	}
	return self;
}

- (void)loadView {
	UIView *aView = [[UIView alloc] init];
	
	table = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
	table.delegate = self;
	table.dataSource = self;
	
	[aView addSubview:table];
	
	self.view = aView;
	[aView release];
}

//- (void)viewDidLoad {
//	CGRect frame = CGRectMake(0.0f, 0.0f, 320.0f, 160.0f);
//	self.table.frame = frame;
//	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
//	
//	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
//	self.navigationItem.leftBarButtonItem = doneButton;
//	[doneButton release];
//	
//	[super viewDidLoad];
//}

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
	
	//table.frame = viewFrame;
	//table.delegate = self;
	//table.dataSource = self;
	
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



//- (void)done:(id)sender {
//	[[mTraderCommunicator sharedManager] stopStreamingData];
//	[mTraderCommunicator sharedManager].symbolsDelegate = nil;
//
//	[self.delegate orderBookControllerDidFinish:self];
//}

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

