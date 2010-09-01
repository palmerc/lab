//
//  mTraderAppDelegate_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

#define DEBUG 0
#define DEBUG_PUSH_NOTIFICATIONS 0

#import "mTraderAppDelegate_Phone.h"

#import "MyListHeaderViewController_Phone.h"
#import "MyListNavigationController_Phone.h"
#import "NewsTableViewController_Phone.h"
#import "SettingsTableViewController_Phone.h"
#import "StatusController.h"

#import "Monitor.h"
#import "DataController.h"
#import "Reachability.h"
#import "UserDefaults.h"


@interface mTraderAppDelegate_Phone ()
- (NSString *)applicationDocumentsDirectory;
@end


@implementation mTraderAppDelegate_Phone
@synthesize window = _window;
@synthesize tabController = _tabController;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize managedObjectModel = _managedObjectModel;
@synthesize persistentStoreCoordinator = _persistentStoreCoordinator;

// +initialize is invoked before the class receives any other messages, so it
// is a good place to set up application defaults
+ (void)initialize {
	NSArray *keys = [NSArray arrayWithObjects:@"username", @"password", nil];
	NSArray *values = [NSArray arrayWithObjects:@"", @"", nil];
		
	NSDictionary *resourceDict = [NSDictionary dictionaryWithObjects:values	forKeys:keys];
	[[NSUserDefaults standardUserDefaults] registerDefaults:resourceDict];
}

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	NSString *applicationName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
	UIColor *sebColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	
	CGRect applicationFrame = [[UIScreen mainScreen] applicationFrame];

	_managedObjectContext = nil;
	_managedObjectModel = nil;
	_persistentStoreCoordinator = nil;

	applicationFrame.size.height -= _tabController.tabBar.frame.size.height;	
	
	// My List
	MyListHeaderViewController_Phone *rootViewController = [[MyListHeaderViewController_Phone alloc] initWithFrame:applicationFrame];
	rootViewController.managedObjectContext = self.managedObjectContext;
	MyListNavigationController_Phone *myListNavigationController = [[MyListNavigationController_Phone alloc] initWithContentViewController:(UIViewController *)rootViewController];
	rootViewController.navigationController = myListNavigationController;
	
	[rootViewController release];
	
	// News
	NewsTableViewContoller_Phone *newsController = [[NewsTableViewContoller_Phone alloc] initWithFrame:applicationFrame];
	newsController.managedObjectContext = self.managedObjectContext;
	UINavigationController *newsNavigationController = [[UINavigationController alloc] initWithRootViewController:newsController];
	if ([applicationName isEqualToString:BRANDING_SEB]) {
		newsNavigationController.navigationBar.tintColor = sebColor;
	}
	[newsController release];

	// Settings
	SettingsTableViewController_Phone *settingsController = [[SettingsTableViewController_Phone alloc] init];
	UINavigationController *settingsNavigationController = [[UINavigationController alloc] initWithRootViewController:settingsController];
	if ([applicationName isEqualToString:BRANDING_SEB]) {
		settingsNavigationController.navigationBar.tintColor = sebColor;
	}
	[settingsController release];
	
	NSArray *viewControllersArray = [NSArray arrayWithObjects:myListNavigationController, newsNavigationController, settingsNavigationController, nil];

	[myListNavigationController release];
	[newsNavigationController release];
	[settingsNavigationController release];
	
	
	DataController *dataController = [DataController sharedManager];
	dataController.managedObjectContext = self.managedObjectContext;
	[dataController deleteAllBidsAsks];	
	
	_tabController = [[UITabBarController alloc] init];
		
	CGRect statusFrame = _tabController.tabBar.frame;
	StatusController *statusController = [[StatusController alloc] initWithFrame:statusFrame];
	[_tabController.view insertSubview:statusController.view belowSubview:_tabController.tabBar];
	
	self.tabController.viewControllers = viewControllersArray;
	
	Monitor *monitor = [Monitor sharedManager];
	monitor.statusController = statusController;	
	[monitor applicationDidFinishLaunching];
	
	CGRect windowFrame = [[UIScreen mainScreen] bounds];
	_window = [[UIWindow alloc] initWithFrame:windowFrame];
	[_window addSubview:_tabController.view];
	[_window makeKeyAndVisible];
	
#if DEBUG_PUSH_NOTIFICATIONS
	NSLog(@"AppDelegate: Registering for push notifications...");
#endif
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeAlert |
																		   UIRemoteNotificationTypeBadge |
																		   UIRemoteNotificationTypeSound)];	
	
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
	[[Monitor sharedManager] applicationWillResignActive];
	
	[[UserDefaults sharedManager] saveSettings];
	
	NSError *error;
    if (_managedObjectContext != nil) {
        if ([_managedObjectContext hasChanges] && ![_managedObjectContext save:&error]) {
			// Update to handle the error appropriately.
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
			abort();
#endif
        } 
    }
}

/**
 applicationWillTerminate: saves changes in the application's managed object context before the application terminates.
 */
- (void)applicationWillTerminate:(UIApplication *)application {
	[[Monitor sharedManager] applicationWillTerminate];
	
	[[UserDefaults sharedManager] saveSettings];
	
	NSError *error;
    if (_managedObjectContext != nil) {
        if ([_managedObjectContext hasChanges] && ![_managedObjectContext save:&error]) {
			// Update to handle the error appropriately.
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
			abort();
#endif
        } 
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	[[Monitor sharedManager] applicationDidBecomeActive];
}

#pragma mark -
#pragma mark Remote Notification Methods

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
	_deviceToken = [deviceToken retain];
	[[UserDefaults sharedManager] setDeviceToken:deviceToken];
	
#if DEBUG_PUSH_NOTIFICATIONS
	NSString *previous = [NSString stringWithFormat:@"Stored device token=%@", [UserDefaults sharedManager].deviceToken];
	NSLog(@"AppDelegate: %@", previous);
	
	NSString *message = [NSString stringWithFormat:@"Received device token=%@", deviceToken];
	NSLog(@"AppDelegate: %@", message);
#endif
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
#if DEBUG_PUSH_NOTIFICATIONS
	NSString *message = [NSString stringWithFormat:@"Error: %@", error];
	NSLog(@"AppDelegate: %@", message);
#endif
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
#if DEBUG_PUSH_NOTIFICATIONS
	for (id key in userInfo) {
		NSLog(@"AppDelegate: key: %@, value: %@", key, [userInfo objectForKey:key]);
	}
#endif
}

#pragma mark -
#pragma mark Core Data stack

/**
 Returns the managed object context for the application.
 If the context doesn't already exist, it is created and bound to the persistent store coordinator for the application.
 */
- (NSManagedObjectContext *)managedObjectContext {
    if (_managedObjectContext != nil) {
        return _managedObjectContext;
    }
	
    NSPersistentStoreCoordinator *coordinator = self.persistentStoreCoordinator;
    if (coordinator != nil) {
        _managedObjectContext = [[NSManagedObjectContext alloc] init];
        [_managedObjectContext setPersistentStoreCoordinator:coordinator];
    }
    return _managedObjectContext;
}


/**
 Returns the managed object model for the application.
 If the model doesn't already exist, it is created by merging all of the models found in the application bundle.
 */
- (NSManagedObjectModel *)managedObjectModel {
    if (_managedObjectModel != nil) {
        return _managedObjectModel;
    }
	
	NSString *modelPath = [[NSBundle mainBundle] pathForResource:@"DataModel" ofType:@"momd"];
	NSAssert(modelPath != nil, @"Managed Object Model is not found.");
	NSURL *modelURL = [NSURL fileURLWithPath:modelPath];
    _managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];    
    
	return _managedObjectModel;
}



/**
 Returns the persistent store coordinator for the application.
 If the coordinator doesn't already exist, it is created and the application's store added to it.
 */
- (NSPersistentStoreCoordinator *)persistentStoreCoordinator {
    if (_persistentStoreCoordinator != nil) {
        return _persistentStoreCoordinator;
    }
		
	NSURL *storeUrl = [NSURL fileURLWithPath:[[self applicationDocumentsDirectory] stringByAppendingPathComponent:@"mTrader02.sqlite"]];
	
	NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
							 [NSNumber numberWithBool:YES], NSMigratePersistentStoresAutomaticallyOption, nil];	
    _persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:self.managedObjectModel];
	
	NSError *error;
	if (![_persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeUrl options:options error:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Could not start persistent store: %@, %@", error, [error userInfo]);
#if DEBUG
		abort();  // Fail
#endif
    }    
	
    return _persistentStoreCoordinator;
}


#pragma mark -
#pragma mark Application's Documents directory

/**
 Returns the path to the application's documents directory.
 */
- (NSString *)applicationDocumentsDirectory {
	
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
    return basePath;
}

#pragma mark -
#pragma mark Debugging methods

#if DEBUG
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
    NSLog(@"mTraderAppDelegate_Phone Queried about %@", NSStringFromSelector(sel));
    return [super respondsToSelector:sel];
}
#endif

#pragma mark -
#pragma mark Memory managment

-(void) dealloc {
	[_deviceToken release];
	[_managedObjectContext release];
    [_managedObjectModel release];
    [_persistentStoreCoordinator release];
	
	[_tabController release];
	[_window release];
	
    [super dealloc];
}

@end
