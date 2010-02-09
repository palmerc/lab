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
@synthesize managedObjectContext, chainsTableViewController;

- (id)init {
	self = [super init];
	if (self != nil) {
		UIImage* anImage = [UIImage imageNamed:@"myStocksTabButton.png"];
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"ChainsTab", @"Chains tab label") image:anImage tag:CHAINS];
		self.tabBarItem = theItem;
		[theItem release];
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

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	chainsTableViewController = [[ChainsTableViewController alloc] init];
	chainsTableViewController.managedObjectContext = self.managedObjectContext;
	
	[self pushViewController:chainsTableViewController animated:NO];
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
	[chainsTableViewController release];
	
    [super dealloc];
}


@end
