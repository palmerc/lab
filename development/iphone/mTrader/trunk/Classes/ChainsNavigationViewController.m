//
//  ChainsNavigationViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "ChainsNavigationViewController.h"

#import "UIToolbarController.h"
#import "mTraderAppDelegate.h"
#import "ChainsTableViewController.h"


@implementation ChainsNavigationViewController
@synthesize chainsTableViewController = _chainsTableViewController;
@synthesize toolBar = _toolBar;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

/*
	NSArray *centerItems = [NSArray arrayWithObjects:@"Last", @"Bid", @"Ask", nil];
	UISegmentedControl *centerControl = [[UISegmentedControl alloc] initWithItems:centerItems];
	centerControl.segmentedControlStyle = UISegmentedControlStyleBar;
	centerControl.selectedSegmentIndex = 0;
	[centerControl addTarget:self action:@selector(centerSelection:) forControlEvents:UIControlEventValueChanged];
	 
	unichar upDownArrowsChar = 0x21C5;
	NSString *upDownArrows = [NSString stringWithCharacters:&upDownArrowsChar length:1];
	NSArray *rightItems = [NSArray arrayWithObjects: @"%", upDownArrows, @"Last", nil];
	UISegmentedControl *rightControl = [[UISegmentedControl alloc] initWithItems:rightItems];
	rightControl.segmentedControlStyle = UISegmentedControlStyleBar;
	rightControl.selectedSegmentIndex = 0;
	[rightControl addTarget:self action:@selector(rightSelection:) forControlEvents:UIControlEventValueChanged];
	 
	UIBarButtonItem *centerBarItem = [[UIBarButtonItem alloc] initWithCustomView:centerControl];
	UIBarButtonItem *rightBarItem = [[UIBarButtonItem alloc] initWithCustomView:rightControl];
	[centerControl release];
	[rightControl release];
	[toolBar setItems:[NSArray arrayWithObjects:centerBarItem, rightBarItem, nil]];
	[self.parentViewController.view addSubview:toolBar];
	[toolBar release];	
*/	
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
	[self.chainsTableViewController release];
	
    [super dealloc];
}


@end
