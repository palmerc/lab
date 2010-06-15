//
//  SymbolSearchController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.06.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "SymbolSearchController_Phone.h"

#import "mTraderCommunicator.h"

@implementation SymbolSearchController_Phone
@synthesize searchResults = _searchResults;

- (id)init {
	self = [super initWithStyle:UITableViewStylePlain];
	if (self != nil) {
		_searchResults = nil;
		
		communicator = [mTraderCommunicator sharedManager];
	}
	return self;
}

#pragma mark -
#pragma mark UISearchDisplayController Delegate Methods

- (void)searchResultsUpdate:(NSArray *)results {
	if (results == nil) {
		self.searchResults = [NSArray array];
	} else {
		self.searchResults = results;
	}

	[self.searchDisplayController.searchResultsTableView reloadData];
}

- (void)searchBarSearchButtonClicked:(UISearchBar *)searchBar {
	[searchBar resignFirstResponder];
}

- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchString:(NSString *)searchString {
	[communicator symbolSearch:searchString];
	return NO;
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

- (void)dealloc {
	[_searchResults release];
	
	[super dealloc];
}

@end
