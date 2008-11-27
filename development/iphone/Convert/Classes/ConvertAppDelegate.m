//
//  ConvertAppDelegate.m
//  Convert
//
//  Created by Cameron Palmer on 15/08/2008.
//  Copyright University of North Texas 2008. All rights reserved.
//

#import "ConvertAppDelegate.h"
#import "RootViewController.h"

@implementation ConvertAppDelegate


@synthesize window;
@synthesize rootViewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {
	
	[window addSubview:[rootViewController view]];
	[window makeKeyAndVisible];
}


- (void)dealloc {
	[rootViewController release];
	[window release];
	[super dealloc];
}

@end
