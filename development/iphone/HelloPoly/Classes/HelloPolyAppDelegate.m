//
//  HelloPolyAppDelegate.m
//  HelloPoly
//
//  Created by Cameron Lowell Palmer on 26.11.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "HelloPolyAppDelegate.h"
#import "MainViewController.h"


@implementation HelloPolyAppDelegate

@synthesize window;
@synthesize mainViewController;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	MainViewController *aController = [[MainViewController alloc] initWithNibName:@"MainView" bundle:nil];
	self.mainViewController = aController;
	[aController release];

    mainViewController.view.frame = [UIScreen mainScreen].applicationFrame;
	[window addSubview:[mainViewController view]];

	int defaultNumberOfSides = [[NSUserDefaults standardUserDefaults] integerForKey:@"numberOfSides"];
	if ( defaultNumberOfSides == 0 ) {
		defaultNumberOfSides = 5;
	}
	int defaultSolidOrDashed = [[NSUserDefaults standardUserDefaults] integerForKey:@"solidOrDashed"];
	mainViewController.numberOfSides = defaultNumberOfSides;
	mainViewController.solidOrDashedIndex = defaultSolidOrDashed;
	[window makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application {
	[[NSUserDefaults standardUserDefaults] setInteger:mainViewController.numberOfSides forKey:@"numberOfSides"];
	[[NSUserDefaults standardUserDefaults] setInteger:mainViewController.solidOrDashedIndex forKey:@"solidOrDashed"];
}

- (void)dealloc {
	[mainViewController release];
    [window release];
    [super dealloc];
}


@end
