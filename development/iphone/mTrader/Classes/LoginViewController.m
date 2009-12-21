//
//  LoginViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "LoginViewController.h"
#import "Communicator.h"

@implementation LoginViewController

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
	comm = [[Communicator alloc] initWithSocket:@"wireless.theonlinetrader.com" port:7780];
	[comm setDelegate:self];
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
	[comm release];
    [super dealloc];
}

- (IBAction)login:(id)sender {
	
	//BOOL result = [session login];
	//if (result == YES) {	
	//	statusLabel.text = @"Success";
	//} else {
//		statusLabel.text = @"Failure";
	//}
	[comm write:@"\r\n\r\nAction: login\r\nAuthorization: cameron/Ct1gg3rR\r\n"];
	
}

- (void)dataReceived {
	NSLog(@"%@", comm.read);
}

@end
