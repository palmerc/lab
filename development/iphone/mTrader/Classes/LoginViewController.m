//
//  LoginViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "LoginViewController.h"
#import "iTraderCommunicator.h"

@implementation LoginViewController
@synthesize usernameTextField;
@synthesize passwordTextField;
@synthesize activityIndicator;
@synthesize loginButton;

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
	iTrader = [[iTraderCommunicator alloc] init];
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

- (IBAction)login:(id)sender {
	[usernameTextField resignFirstResponder];
	[passwordTextField resignFirstResponder];
	
	activityIndicator.hidden = NO;
	[activityIndicator startAnimating];
	loginButton.enabled = NO;
	
	[iTrader login:usernameTextField.text password:passwordTextField.text];
	
	while (![iTrader loginStatusHasChanged] && [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]]);
	
	[activityIndicator stopAnimating];
	activityIndicator.hidden = YES;
	if (iTrader.isLoggedIn == NO) {
		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Login Failure" message:@"Login to the server failed. If you believe this is in error try again or contact support." delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
		[alertView show];
		[alertView release];
		loginButton.enabled = YES; 
	} else {
		[self.view removeFromSuperview];
	}
}

@end
	