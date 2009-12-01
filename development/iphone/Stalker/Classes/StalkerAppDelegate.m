//
//  StalkerAppDelegate.m
//  Stalker
//
//  Created by Cameron Lowell Palmer on 27.11.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "StalkerAppDelegate.h"

@implementation StalkerAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    

    // Override point for customization after application launch
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
