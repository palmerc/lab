//
//  iTraderAppDelegate.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

#import "iTraderAppDelegate.h"
#import "Starter.h"
#import "MyStocksViewController.h"
#import "NewsViewController.h"
#import "SettingsTableViewController.h"

@implementation iTraderAppDelegate
@synthesize window;
@synthesize tabController;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	// Start up the Singleton services
	Starter *starter = [[Starter alloc] init];
	[starter release];
	
	defaults = [[NSUserDefaults alloc] init];
	// if username and password are empty make the default starting tab the Settings Tab

	window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	window.backgroundColor = [UIColor lightGrayColor];
	
	myStocks = [[MyStocksViewController alloc] init];
	news = [[NewsViewController alloc] init];
	SettingsTableViewController *settings = [[SettingsTableViewController alloc] init];
	settingsNavigationController = [[UINavigationController alloc] initWithRootViewController:settings];

	NSArray *viewControllersArray = [NSArray arrayWithObjects:myStocks, news, settingsNavigationController, nil];
	
	tabController = [[UITabBarController alloc] init];
	self.tabController.viewControllers = viewControllersArray;
	[window addSubview:tabController.view];
	[window makeKeyAndVisible];
}


- (void)dealloc {
	[defaults release];
	[communicator release];
    [window release];
    [super dealloc];
}


@end
