//
//  GradientAppDelegate.m
//  Gradient
//
//  Created by Cameron Lowell Palmer on 22.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//

#import "GradientAppDelegate.h"
#import "GradientViewController.h"

@implementation GradientAppDelegate

@synthesize window;
@synthesize viewController;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	CGRect statusBarFrame = application.statusBarFrame;
	CGRect windowFrame = [[UIScreen mainScreen] bounds];
	
	windowFrame.origin.y = statusBarFrame.size.height;
    window = [[UIWindow alloc] initWithFrame:windowFrame];
	viewController = [[GradientViewController alloc] init];
    
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
	
	return YES;
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
