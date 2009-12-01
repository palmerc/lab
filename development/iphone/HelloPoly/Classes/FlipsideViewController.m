//
//  FlipsideViewController.m
//  HelloPoly
//
//  Created by Cameron Lowell Palmer on 29.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "FlipsideViewController.h"


@implementation FlipsideViewController

@synthesize delegate;
@synthesize segmentedControlIndex;

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

- (void)viewDidLoad {
	[super viewDidLoad];
	solidOrDashedSegmentedControl.selectedSegmentIndex = segmentedControlIndex;

}


- (IBAction)done {
	[self.delegate flipsideViewControllerDidFinish:self];	
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

- (IBAction)solidOrDashed:(id)sender {
	segmentedControlIndex = [(UISegmentedControl *)sender selectedSegmentIndex];
	[self.delegate setSolidOrDashed:segmentedControlIndex];
}

@end
