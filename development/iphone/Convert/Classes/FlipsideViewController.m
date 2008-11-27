//
//  FlipsideViewController.m
//  Convert
//
//  Created by Cameron Palmer on 15/08/2008.
//  Copyright University of North Texas 2008. All rights reserved.
//

#import "FlipsideViewController.h"


@implementation FlipsideViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		// Custom initialization
	}
	return self;
}
/*- (void)viewDidLoad {
	self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];		
}*/


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc {
	[super dealloc];
}


@end
