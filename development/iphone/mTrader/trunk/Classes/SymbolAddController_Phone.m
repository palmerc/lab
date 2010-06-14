//
//  SymbolAddController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolAddController_Phone.h"

#import "Feed.h"
#import "QFields.h"
#import "mTraderCommunicator.h"

@implementation SymbolAddController_Phone
@synthesize delegate;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize searchResults = _searchResults;
 
#pragma mark -
#pragma mark Application Lifecycle

- (id)initWithFrame:(CGRect)frame {
	self = [super init];
	if (self != nil) {
		_frame = frame;
		_searchResults = nil;
	}
	return self;
}

- (void)loadView {
	UIView *aView = [[UIView alloc] initWithFrame:_frame];
	aView.backgroundColor = [UIColor whiteColor];
	
	UIBarButtonItem *cancel = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancel:)];
	self.navigationItem.leftBarButtonItem = cancel;
	[cancel release];
	
	CGRect searchFrame = aView.bounds;
	//searchFrame.size.height = 44.0f;
	UISearchBar *searchBar = [[UISearchBar alloc] initWithFrame:searchFrame];
	searchBar.delegate = self;
	searchBar.autocapitalizationType = UITextAutocapitalizationTypeNone;
	searchBar.placeholder = NSLocalizedString(@"tickerSymbolOrName", @"Ticker Symbol or Name");
	
	[aView addSubview:searchBar];
	[searchBar release];
	
	self.view = aView;
	[aView release];
}

- (void)viewDidLoad {
	self.title = NSLocalizedString(@"addASymbol", @"Add a Symbol");
	communicator = [mTraderCommunicator sharedManager];
	
	[DataController sharedManager].searchDelegate = self;
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
