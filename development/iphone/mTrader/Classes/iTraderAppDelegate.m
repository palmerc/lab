//
//  mTraderAppDelegate.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "iTraderAppDelegate.h"
#import "LoginViewController.h"

@implementation iTraderAppDelegate

@synthesize window;
@synthesize tabBarController;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	tabBarController = [[UITabBarController alloc] initWithNibName:@"TabController" bundle:nil];
	[window addSubview:tabBarController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
	[tabBarController release];
    [window release];
    [super dealloc];
}


@end
