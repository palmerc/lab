//
//  BBLinksAppDelegate.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "BBLinksAppDelegate.h"
#import "BBLinksViewController.h"

@implementation BBLinksAppDelegate

@synthesize window;
@synthesize viewController;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	window = [[UIWindow alloc] init];
    viewController = [[BBLinksViewController alloc] init];	
	
    // Override point for customization after app launch    
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
	
	[viewController release];

	return YES;
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
