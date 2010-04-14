//
//  SymbolAddController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolAddController.h"

#import "Feed.h"
#import "QFields.h"
#import "mTraderCommunicator.h"



@implementation SymbolAddController
@synthesize delegate;
 
#pragma mark -
#pragma mark Application Lifecycle

- (void)viewDidLoad {
	self.title = @"Add a Symbol";
	communicator = [mTraderCommunicator sharedManager];
	
	UIBarButtonItem *cancel = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancel:)];
	self.navigationItem.leftBarButtonItem = cancel;
	[cancel release];
	
	CGRect searchFrame = self.view.bounds;
	searchFrame.size.height = 44.0f;
	UISearchBar *searchBar = [[UISearchBar alloc] initWithFrame:searchFrame];
	searchBar.showsCancelButton = NO;
	searchBar.autocorrectionType = UITextAutocorrectionTypeNo;
	searchBar.autocapitalizationType = UITextAutocapitalizationTypeNone;
	searchBar.placeholder = @"Ticker Symbol or Name";
	searchController = [[UISearchDisplayController alloc] initWithSearchBar:searchBar contentsController:self];
	
	searchController.delegate = self;
	searchController.searchResultsDataSource = self;
	searchController.searchResultsDelegate = self;
	
	[self.view addSubview:searchBar];
	[searchBar release];
}

- (void)viewWillAppear:(BOOL)animated {
	[self changeQFieldsStreaming];
}

- (void)changeQFieldsStreaming {	
	QFields *qFields = [[QFields alloc] init];
	
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:nil];
}

- (void)dealloc {
	[searchController release];
	
	[super dealloc];
}

- (void)cancel:(id)sender {
	[self.delegate symbolAddControllerDidFinish:self didAddSymbol:nil];
}

#pragma mark -
#pragma mark UISearchDisplayController Delegate Methods

- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchString:(NSString *)searchString {
	NSLog(@"%@", searchString);
	[communicator symbolSearch:searchString];
	
	return NO;
}


#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	 NSLog(@"Queried about %@", NSStringFromSelector(sel));
	 return [super respondsToSelector:sel];
}



- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	static NSString *CellIdentifier = @"SearchTableCell";
    
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
    // Configure the cell.
    return cell;	
}

- (NSInteger)tableView:(UITableView *)table numberOfRowsInSection:(NSInteger)section {
	return 0;
}

@end
