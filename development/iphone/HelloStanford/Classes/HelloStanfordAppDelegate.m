//
//  HelloStanfordAppDelegate.m
//  HelloStanford
//
//  Created by Cameron Lowell Palmer on 19.11.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "HelloStanfordAppDelegate.h"

@implementation HelloStanfordAppDelegate

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
