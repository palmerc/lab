//
//  PresenceAppDelegate.m
//  Presence
//
//  Created by Cameron Lowell Palmer on 09.12.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "PresenceAppDelegate.h"
#import "PersonListViewController.h"

@implementation PresenceAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	controller = [[UINavigationController alloc] init];
	PersonListViewController *personListVC = [[PersonListViewController alloc] initWithStyle:UITableViewStylePlain];
	[controller pushViewController:personListVC animated:NO];
	[personListVC release];
	
    // Override point for customization after application launch
    [window addSubview:controller.view];
	[window makeKeyAndVisible];
}


- (void)dealloc {
	[controller release];
    [window release];
    [super dealloc];
}


@end
