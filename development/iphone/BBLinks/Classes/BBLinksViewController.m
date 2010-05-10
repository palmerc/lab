//
//  BBLinksViewController.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "BBLinksViewController.h"
#import "BBTextLabel.h"

@implementation BBLinksViewController

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	[self.view setBackgroundColor:[UIColor whiteColor]];
	
	BBTextLabel *tl = [[BBTextLabel alloc] initWithFrame:CGRectMake(0.0, 0.0, 100.0, 100.0)];
	tl.text = @"This is a link.";
	[self.view addSubview:tl];
	
}


// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

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

@end
