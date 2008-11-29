//
//  HelloAppDelegate.m
//  Hello
//
//  Created by Cameron Palmer on 28/11/2008.
//  Copyright University of North Texas 2008. All rights reserved.
//

#import "HelloAppDelegate.h"
#import "HelloViewController.h"

@implementation HelloAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    
    // Override point for customization after app launch    
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
