//
//  NetworkDiagnosticAppDelegate.m
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 09.04.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "NetworkDiagnosticAppDelegate.h"
#import "NetworkDiagnosticViewController.h"

@implementation NetworkDiagnosticAppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	CGRect frame = [[UIScreen mainScreen] bounds];
    _window = [[UIWindow alloc] initWithFrame:frame];
	//_window.autoresizesSubviews = YES;
	//_window.autoresizingMask = UIViewAutoresizingFlexibleHeight;
	
	_viewController = [[NetworkDiagnosticViewController alloc] initWithFrame:frame];

	// Override point for customization after app launch    
    [self.window addSubview:_viewController.view];
    [self.window makeKeyAndVisible];
	
	return YES;
}


- (void)dealloc {
    [_viewController release];
    [_window release];
    [super dealloc];
}


@end
