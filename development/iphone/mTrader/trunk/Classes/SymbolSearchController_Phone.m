//
//  SymbolSearchController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.06.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "SymbolSearchController_Phone.h"

#import "mTraderCommunicator.h"

@implementation SymbolSearchController_Phone
@synthesize searchResults = _searchResults;
@synthesize searchBar = _searchBar;

#pragma mark -
#pragma mark Initialization

- (id)init {
	self = [super initWithStyle:UITableViewStylePlain];
	if (self != nil) {
		_searchResults = nil;
		
		_coveringView = nil;
		
		communicator = [mTraderCommunicator sharedManager];
	}
	return self;
}

#pragma mark -
#pragma mark UIViewController Delegate Methods

- (void)viewDidLoad {
	_coveringView = [[SymbolSearchCoveringView_Phone alloc] initWithFrame:self.view.bounds];
	_coveringView.delegate = self;
	[self.view addSubview:_coveringView];
}

#pragma mark -
#pragma mark SymbolSearchCoveringView_Phone Delegate Methods

- (void)coveringView:(id)sender didReceiveTouches:(NSSet *)touches withEvent:(UIEvent *)event {
	SymbolSearchCoveringView_Phone *coveringView = (SymbolSearchCoveringView_Phone *)sender;
	coveringView.touchesEnabled = NO;
	[self.searchBar resignFirstResponder];
}

#pragma mark -
#pragma mark DataController Delegate Methods

- (void)searchResultsUpdate:(NSMutableArray *)results {
	if (results == nil) {
		self.searchResults = [NSArray array];
	} else {
		self.searchResults = results;
	}

	[self.tableView reloadData];
}

- (void)searchBarSearchButtonClicked:(UISearchBar *)searchBar {
	[searchBar resignFirstResponder];
}

- (void)searchBarTextDidBeginEditing:(UISearchBar *)searchBar {
	_coveringView.touchesEnabled = YES;
}

- (void)searchBar:(UISearchBar *)searchBar textDidChange:(NSString *)searchText {
	[communicator symbolSearch:searchText];
}

#pragma mark -
#pragma mark UITableView Delegate Methods

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	static NSString *CellIdentifier = @"SearchTableCell_Phone";
    
    UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
    }
	
	NSArray *row = [self.searchResults objectAtIndex:indexPath.row];
	
	NSString *feedTicker = [row objectAtIndex:0];
	NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	NSString *mCode = [row objectAtIndex:1];
	NSString *description = [row objectAtIndex:2];
	cell.textLabel.text = [NSString stringWithFormat:@"%@ (%@)", tickerSymbol, mCode];
	cell.detailTextLabel.text = [NSString stringWithFormat:@"%@", description];
    return cell;	
}

- (NSInteger)tableView:(UITableView *)table numberOfRowsInSection:(NSInteger)section {
	return [self.searchResults count];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NSArray *row = [self.searchResults objectAtIndex:indexPath.row];
	
	NSString *feedTicker = [row objectAtIndex:0];
	NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	NSString *mCode = [row objectAtIndex:1];	
	
	[communicator addSecurity:tickerSymbol withMCode:mCode];
	
	[self.searchResults removeObjectAtIndex:indexPath.row];
	NSArray *indexPaths = [NSArray arrayWithObject:indexPath];
	[tableView deleteRowsAtIndexPaths:indexPaths withRowAnimation:YES];
}

#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
#if DEBUG
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

- (void)dealloc {
	[_searchResults release];
	[_coveringView release];
	[super dealloc];
}

@end
