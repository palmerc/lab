//
//  SymbolNewsController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "SymbolNewsController.h"

#import "Feed.h"
#import "Symbol.h"
#import "SymbolNewsCell.h"
#import "SymbolNewsItemController.h"

@implementation SymbolNewsController
@synthesize delegate;
@synthesize communicator;
@synthesize symbol = _symbol;

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
	self = [super init];
    if (self != nil) {
		self.symbol = symbol;
		newsArray = nil;
		
		table = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
		table.delegate = self;
		table.dataSource = self;
		
		[self.view addSubview:table];
	}
    return self;
}

- (void)viewWillAppear:(BOOL)animated {
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	
	communicator = [mTraderCommunicator sharedManager];
	communicator.symbolsDelegate = self;
	[communicator symbolNewsForFeedTicker:feedTicker];
}

- (void)viewDidLoad {
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
		
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
	
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
	
	table.frame = self.view.bounds;
		
	[super viewDidLoad];
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

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

#pragma mark -
#pragma mark TableView datasource methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [newsArray count];
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"SymbolSymbolNewsCell";
    
    SymbolNewsCell *cell = (SymbolNewsCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[SymbolNewsCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(SymbolNewsCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	NSInteger row = indexPath.row;
	NSArray *newsItem = [newsArray objectAtIndex:row];
	cell.newsItem = newsItem;
}

#pragma mark -
#pragma mark TableView delegate methods

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	[tableView deselectRowAtIndexPath:indexPath animated:YES];

	SymbolNewsItemController *newsItemController = [[SymbolNewsItemController alloc] init];
	
	NSArray *articleArray = [newsArray objectAtIndex:indexPath.row];
	NSString *feedArticle = [articleArray objectAtIndex:0];
	newsItemController.feedArticle = feedArticle;
	
	[self.navigationController pushViewController:newsItemController animated:YES];
	
	[newsItemController release];
}

#pragma mark -
#pragma mark Delegation

-(void) newsListFeedsUpdates:(NSArray *)newsList {
	if (newsArray != nil) {
		[newsArray release];
		newsArray = nil;
	}
	
	NSMutableArray *tempStorage = [[[NSMutableArray alloc] init] autorelease];
	for (NSString *news in newsList) {
		NSArray *components = [news componentsSeparatedByString:@";"];
		if ([components count] >= 4) {
			NSString *feedArticle = [components objectAtIndex:0];
			NSString *flag = [components objectAtIndex:1];
			NSString *date = [components objectAtIndex:2];
			NSString *time = [components objectAtIndex:3];
			NSString *headline = [components objectAtIndex:4];
			NSArray *tuple = [NSArray arrayWithObjects:feedArticle, flag, date, time, headline, nil];
			
			[tempStorage addObject:tuple];
		}
	}
	
	newsArray = (NSArray *)tempStorage;
	[newsArray retain];
	
	[table reloadData];
}

#pragma mark -
#pragma mark Actions

- (void)done:(id)sender {
	[self.delegate symbolNewsControllerDidFinish:self];
}

- (void)refresh:(id)sender {
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];

	[communicator symbolNewsForFeedTicker:feedTicker];
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in SymbolNewsItemView", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[table release];
	[newsArray release];
	[_symbol release];
    [super dealloc];
}


@end
