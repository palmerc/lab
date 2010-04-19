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
@synthesize managedObjectContext = _managedObjectContext;
@synthesize searchResults = _searchResults;
 
#pragma mark -
#pragma mark Application Lifecycle

- (void)viewDidLoad {
	self.title = @"Add a Symbol";
	self.view.backgroundColor = [UIColor whiteColor];
	
	communicator = [mTraderCommunicator sharedManager];
	
	[SymbolDataController sharedManager].searchDelegate = self;
	_searchResults = nil;
	
	UIBarButtonItem *cancel = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancel:)];
	self.navigationItem.leftBarButtonItem = cancel;
	[cancel release];
	
	CGRect searchFrame = self.view.bounds;
	searchFrame.size.height = 44.0f;
	UISearchBar *searchBar = [[UISearchBar alloc] initWithFrame:searchFrame];
	searchBar.delegate = self;
	searchBar.autocapitalizationType = UITextAutocapitalizationTypeNone;
	searchBar.placeholder = @"Ticker Symbol or Name";

	[self.view addSubview:searchBar];
	[searchBar release];
}

- (void)viewDidUnload {
	[SymbolDataController sharedManager].searchDelegate = nil;
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
	[_searchResults release];
	
	[super dealloc];
}

- (void)cancel:(id)sender {
	[self.delegate symbolAddControllerDidFinish:self didAddSymbol:nil];
}

#pragma mark -
#pragma mark UISearchDisplayController Delegate Methods

- (void)searchBar:(UISearchBar *)searchBar textDidChange:(NSString *)searchText {
	if (![searchText isEqualToString:@""]) {
		[communicator symbolSearch:searchText];
	} else {
		[self searchResultsUpdate:nil];
	}
}

- (void)searchResultsUpdate:(NSArray *)results {
	if (results == nil) {
		results = [NSArray array];
	} else {
		NSSet *results = [NSSet setWithArray:results];
		
		NSArray *existingSymbols = [[SymbolDataController sharedManager] fetchAllSymbols];	
		for (
		NSPredicate *filter = [NSPredicate predicateWithFormat:@" MATCHES %@" argumentArray:existingSymbols];
	}
	self.searchResults = results;
	[self.tableView reloadData];
}

- (void)searchBarSearchButtonClicked:(UISearchBar *)searchBar {
	[searchBar resignFirstResponder];
}

#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	 NSLog(@"Queried about %@", NSStringFromSelector(sel));
//	 return [super respondsToSelector:sel];
//}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	static NSString *CellIdentifier = @"SearchTableCell";
    
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
    }
	
	NSArray *row = [self.searchResults objectAtIndex:indexPath.row];
	
	cell.textLabel.text = [NSString stringWithFormat:@"%@ (%@)", [row objectAtIndex:1], [row objectAtIndex:0]];
	cell.detailTextLabel.text = [NSString stringWithFormat:@"%@", [row objectAtIndex:2]];
    return cell;	
}

- (NSInteger)tableView:(UITableView *)table numberOfRowsInSection:(NSInteger)section {
	return [self.searchResults count];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NSArray *row = [self.searchResults objectAtIndex:indexPath.row];
	
	[communicator addSecurity:[row objectAtIndex:1] withMCode:[row objectAtIndex:0]];
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

@end
