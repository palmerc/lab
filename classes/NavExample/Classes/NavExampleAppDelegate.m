//
//  NavExampleAppDelegate.m
//  NavExample
//
//  Created by Cameron Lowell Palmer on 05.12.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "NavExampleAppDelegate.h"
#import "FirstViewController.h"

@implementation NavExampleAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	navigationController = [[UINavigationController alloc] init];
	
	FirstViewController *firstViewController = [[FirstViewController alloc] initWithNibName:@"FirstView" bundle:nil];
	[navigationController pushViewController:firstViewController animated:NO];
	
	[firstViewController release];
    [window addSubview:navigationController.view];
	[window makeKeyAndVisible];
}


- (void)dealloc {
	[navigationController release];
    [window release];
    [super dealloc];
}


@end
