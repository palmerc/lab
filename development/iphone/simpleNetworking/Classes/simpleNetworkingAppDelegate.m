//
//  simpleNetworkingAppDelegate.m
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "simpleNetworkingAppDelegate.h"
#import "LoginViewController.h"

@implementation simpleNetworkingAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	loginViewController = [[LoginViewController alloc] init];
    [window addSubview:loginViewController.view];
	[window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
