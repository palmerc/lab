    //
//  StatusController.m
//  Popup
//
//  Created by Cameron Lowell Palmer on 07.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "StatusController.h"

#import "StatusView.h"


@implementation StatusController


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	CGRect applicationFrame = [[UIScreen mainScreen] applicationFrame];
	UIView *aView = [[UIView alloc] initWithFrame:applicationFrame];
	aView.backgroundColor = [UIColor whiteColor];
	CGRect statusFrame = CGRectMake(0.0f, aView.bounds.size.height, 320.0f, 50.0f);
	_statusView = [[StatusView alloc] initWithFrame:statusFrame];
	_statusView.rectColor = [UIColor lightGrayColor];
	_statusView.strokeColor = [UIColor whiteColor];
	
	[aView addSubview:_statusView];
	[_statusView release];
	
	self.view = aView;
	
	[aView release];
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[button addTarget:self action:@selector(test:) forControlEvents:UIControlEventTouchUpInside];
	
	button.frame = CGRectMake(0.0f, 0.0f, 200.0f, 50.0f);
	[button setTitle:@"Test" forState:UIControlStateNormal];
	button.backgroundColor = [UIColor whiteColor];
	
	CGRect bounds = [[UIScreen mainScreen] bounds];
	CGFloat x = floorf(bounds.size.width / 2.0f);
	CGFloat y = floorf(bounds.size.height / 2.0f);
	CGPoint center = CGPointMake(x, y);
	button.center = center;	
	
	[self.view addSubview:button];
}

- (void)displayStatus {
	CGRect destinationFrame = _statusView.frame;
	destinationFrame.origin.y -= 47.0f;
	
	[UIView beginAnimations:@"RiseDarthVader" context:NULL];
	[UIView setAnimationDuration:1.0f];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
	[UIView setAnimationBeginsFromCurrentState:YES];
	_statusView.frame = destinationFrame;
	
	[UIView commitAnimations];
}

- (void)hideStatus {
	CGRect destinationFrame = _statusView.frame;
	destinationFrame.origin.y += 47.0f;
	
	[UIView beginAnimations:@"LowerDarthVader" context:NULL];
	[UIView setAnimationDuration:1.0f];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
	[UIView setAnimationBeginsFromCurrentState:YES];
	_statusView.frame = destinationFrame;
	
	[UIView commitAnimations];	
}

- (void)changeStatusMessage:(NSString *)statusMessage {
	_statusView.message = statusMessage;
}

- (void)test:(id)sender {
	static BOOL state = NO;
	
	if (state == NO) {
		[_statusView.activityIndicator startAnimating];

		[self changeStatusMessage:@"Connecting"];
		[self displayStatus];
		state = YES;
	} else {
		[_statusView.activityIndicator stopAnimating];

		[self changeStatusMessage:@"Connected"];
		[self hideStatus];
		state = NO;
	}
	
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
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
