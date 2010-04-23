//
//  GradientViewController.m
//  GradientLabel
//
//  Created by Cameron Lowell Palmer on 22.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//

#import "GradientViewController.h"

#import "GradientView.h"

@implementation GradientViewController



- (void)loadView {
	[super loadView];
	
	CGRect controllerFrame = self.view.bounds;

	UIView *mainView = [[UIView alloc] initWithFrame:controllerFrame];
	mainView.backgroundColor = [UIColor whiteColor]; // autoreleased
	
	CGRect gViewFrame = controllerFrame;
	gViewFrame.size.height = 40.0f;
	GradientView *gradientView = [[GradientView alloc] initWithFrame:gViewFrame];
	[mainView addSubview:gradientView];
	[gradientView release];
	
	self.view = mainView;
	[mainView release];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)dealloc {
    [super dealloc];
}

@end
