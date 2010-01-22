//
//  iTraderAppDelegate.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

#import "iTraderAppDelegate.h"
#import "Starter.h"
#import "UserDefaults.h"
#import "MyStocksViewController.h"
#import "NewsViewController.h"
#import "SettingsTableViewController.h"

@implementation iTraderAppDelegate
@synthesize window;
@synthesize tabController;

// +initialize is invoked before the class receives any other messages, so it
// is a good place to set up application defaults
+ (void)initialize {
    if ([self class] == [iTraderAppDelegate class]) {
        // Register a default value for the instrument calibration. 
        // This will be used if the user hasn't calibrated the instrument.
		NSArray *keys = [NSArray arrayWithObjects:@"username", @"password", nil];
		NSArray *values = [NSArray arrayWithObjects:@"", @"", nil];
		
		NSDictionary *resourceDict = [NSDictionary dictionaryWithObjects:values	forKeys:keys];
		[[NSUserDefaults standardUserDefaults] registerDefaults:resourceDict];
    }
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	// Start up the Singleton services
	Starter *starter = [[Starter alloc] init];
	[starter release];
	
	defaults = [[NSUserDefaults alloc] init];
	// if username and password are empty make the default starting tab the Settings Tab

	window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	window.backgroundColor = [UIColor lightGrayColor];
	
	MyStocksViewController *myStocks = [[MyStocksViewController alloc] init];
	myStocksNavigationController = [[UINavigationController alloc] initWithRootViewController:myStocks];
	
	NewsViewController *news = [[NewsViewController alloc] init];
	newsNavigationController = [[UINavigationController alloc] initWithRootViewController:news];
	
	SettingsTableViewController *settings = [[SettingsTableViewController alloc] init];
	settingsNavigationController = [[UINavigationController alloc] initWithRootViewController:settings];

	NSArray *viewControllersArray = [NSArray arrayWithObjects:myStocksNavigationController, newsNavigationController, settingsNavigationController, nil];
	
	tabController = [[UITabBarController alloc] init];
	self.tabController.viewControllers = viewControllersArray;
	[window addSubview:tabController.view];
	[window makeKeyAndVisible];
}


-(void) applicationWillTerminate:(UIApplication *)application {
	[[UserDefaults sharedManager] saveSettings];
}

- (void)dealloc {
	[myStocksNavigationController release];
	[newsNavigationController release];
	[settingsNavigationController release];
	
	
	[defaults release];
    [window release];
    [super dealloc];
}


@end
