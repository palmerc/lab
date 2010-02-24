//
//  ChainsNavigationViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "ChainsNavigationViewController.h"

#import "mTraderAppDelegate.h"
#import "ChainsTableViewController.h"


@implementation ChainsNavigationViewController
@synthesize chainsTableViewController = _chainsTableViewController;
@synthesize toolBar = _toolBar;

- (id)initWithContentViewController:(UIViewController *)rootViewController {
	self = [super init];
	if (self != nil) {
		_chainsTableViewController = [rootViewController retain];
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
	
	CGRect frame = contentView.frame;
	UIView *view = [[UIView alloc] initWithFrame:frame];
	
	frame = CGRectMake(0.0f, 0.0f, frame.size.width, frame.size.height - 44.0f);
	contentView.frame = frame;
	[view addSubview:contentView];
	
	frame = CGRectMake(0.0f, frame.size.height - 49.0f, frame.size.width, 44.0f);
	_toolBar = [[UIToolbar alloc] initWithFrame:frame];
	[view addSubview:self.toolBar];
	[self.toolBar release];

	self.view = view;
	[view release];
	((ChainsTableViewController *)self.chainsTableViewController).toolBar = self.toolBar;
	[self pushViewController:self.chainsTableViewController animated:NO];
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
	self.chainsTableViewController = nil;
}


- (void)dealloc {
	[self.toolBar release];
	[self.chainsTableViewController release];

    [super dealloc];
}


@end
