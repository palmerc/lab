//
//  TableDemoAppDelegate.m
//  TableDemo
//
//  Created by Cameron Lowell Palmer on 07.12.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "TableDemoAppDelegate.h"
#import "TableViewController.h"

@implementation TableDemoAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	aTableViewController = [[TableViewController alloc] init];
    // Override point for customization after application launch
	[window addSubview:aTableViewController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
	[aTableViewController release];
    [window release];
    [super dealloc];
}


@end
