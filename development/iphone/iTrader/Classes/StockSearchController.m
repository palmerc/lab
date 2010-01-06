//
//  StockSearchController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "StockSearchController.h"


@implementation StockSearchController
@synthesize delegate, searchBar;

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
 
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	self.searchBar.delegate = self;
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

- (void)dealloc {
    [super dealloc];
}


// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
    NSLog(@"Queried about %@", NSStringFromSelector(sel));
    return [super respondsToSelector:sel];
}

/** 
 * All The Following Methods are Required When Adopting the UISearchBarDelegate 
 * protocol
 */
// Editing Text
- (void)searchBar:(UISearchBar *)searchBar textDidChange:(NSString *)searchText {
	NSLog(@"%@", searchText);
}

- (BOOL)searchBarShouldBeginEditing:(UISearchBar *)searchBar {
	return YES;
}

- (void)searchBarTextDidBeginEditing:(UISearchBar *)searchBar {
}

- (BOOL)searchBarShouldEndEditing:(UISearchBar *)searchBar {
	return YES;
}

- (void)searchBarTextDidEndEditing:(UISearchBar *)searchBar {
	[self.searchBar resignFirstResponder];
}

// Clicking Buttons
- (void)searchBarBookmarkButtonClicked:(UISearchBar *)searchBar {
}

- (void)searchBarCancelButtonClicked:(UISearchBar *)searchBar {
}

- (void)searchBarSearchButtonClicked:(UISearchBar *)searchBar {
	NSLog(@"Search Button Clicked");
	[self.delegate stockSearchControllerDidFinish:self didAddSymbol:nil];
}

// Scope Button
- (void)searchBar:(UISearchBar *)searchBar selectedScopeButtonIndexDidChange:(NSInteger)selectedScope {
}

@end
