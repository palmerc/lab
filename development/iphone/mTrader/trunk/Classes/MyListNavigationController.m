//
//  MyListNavigationController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "MyListNavigationController.h"

#import "mTraderAppDelegate.h"
#import "MyListViewController.h"


@implementation MyListNavigationController
@synthesize myListViewController = _myListViewController;

- (id)initWithContentViewController:(UIViewController *)rootViewController {
	self = [super init];
	if (self != nil) {
		_myListViewController = [rootViewController retain];
		UIImage* anImage = [UIImage imageNamed:@"myStocksTabButton.png"];	
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"MyStocksTab", "My Stocks tab label")  image:anImage tag:CHAINS];
		self.tabBarItem = theItem;
		[theItem release];
	}
	return self;
}

- (void)loadView {
	[super loadView];
	
	UIView *contentView = self.view;
	CGRect frame = self.view.frame;
	UIView *aView = [[UIView alloc] initWithFrame:frame];
	[aView addSubview:contentView];
	[contentView addSubview:self.myListViewController.view];
	
	self.view = aView;
	[aView release];
	[self pushViewController:self.myListViewController animated:NO];
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
	self.myListViewController = nil;
}


- (void)dealloc {
	[_myListViewController release];

    [super dealloc];
}


@end
