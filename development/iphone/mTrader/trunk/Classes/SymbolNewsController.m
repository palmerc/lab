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

@implementation SymbolNewsController
@synthesize delegate;
@synthesize symbol = _symbol;

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
    if (self = [super init]) {
		self.symbol = symbol;
		newsArray = nil;
		
		table = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
		[self.view addSubview:table];
	}
    return self;
}

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

- (void)viewWillAppear:(BOOL)animated {
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	communicator.symbolsDelegate = self;
	[communicator symbolNewsForFeedTicker:feedTicker];
}

- (void)viewDidLoad {
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.rightBarButtonItem = doneButton;
	[doneButton release];
	
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
#pragma mark TableView methods
#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [newsArray count];
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"SymbolNewsCell";
    
    NewsCell *cell = (NewsCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[NewsCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(NewsCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	NSInteger row = indexPath.row;
	News *n = [newsArray objectAtIndex:row];
	
	cell.news = n;
}


#pragma mark -
#pragma mark Delegation

-(void) newsListFeedsUpdates:(NSArray *)newsList {
	if (newsArray != nil) {
		[newsArray release];
	}
	
	NSMutableArray *tempStorage = [[NSMutableArray alloc] init];
	for (NSString *news in newsList) {
		NSArray *components = [news componentsSeparatedByString:@";"];
		if ([components count] >= 4) {
			NSString *key = [components objectAtIndex:0];
			NSString *value = [components objectAtIndex:4];
			NSArray *tuple = [NSArray arrayWithObjects:key, value, nil];
			
			[tempStorage addObject:tuple];
		}
	}
	
	newsArray = (NSArray *)tempStorage;
	[newsArray retain];
	[tempStorage release];
	
	//[self.tableView reloadData];
}

#pragma mark -
#pragma mark Actions

- (void)done:(id)sender {
	[self.delegate symbolNewsControllerDidFinish:self];
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
