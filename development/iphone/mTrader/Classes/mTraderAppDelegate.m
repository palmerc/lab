//
//  mTraderAppDelegate.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "mTraderAppDelegate.h"
#import "LoginViewController.h"

@implementation mTraderAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	loginViewController = [[LoginViewController alloc] initWithNibName:@"LoginView" bundle:nil];
    [window addSubview:loginViewController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
	[loginViewController release];
    [window release];
    [super dealloc];
}


@end
