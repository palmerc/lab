//
//  mTraderAppDelegate_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

#define DEBUG 1

#import "mTraderAppDelegate_Phone.h"

#import "MyListHeaderViewController_Phone.h"
#import "MyListNavigationController_Phone.h"
#import "NewsController.h"
#import "SettingsTableViewController_Phone.h"

#import "mTraderServerMonitor.h"
#import "Reachability.h"
#import "Starter.h"
#import "UserDefaults.h"

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
	CGRect windowFrame = [[UIScreen mainScreen] bounds];
	CGRect applicationFrame = [[UIScreen mainScreen] applicationFrame];

	_managedObjectContext = nil;
	_managedObjectModel = nil;
	_persistentStoreCoordinator = nil;
	_tabController = [[UITabBarController alloc] init];
	applicationFrame.size.height -= _tabController.tabBar.frame.size.height;
	
	// My List
	MyListHeaderViewController_Phone *rootViewController = [[MyListHeaderViewController_Phone alloc] initWithFrame:applicationFrame];
	rootViewController.managedObjectContext = self.managedObjectContext;
	MyListNavigationController_Phone *myListNavigationController = [[MyListNavigationController_Phone alloc] initWithContentViewController:(UIViewController *)rootViewController];
	[rootViewController release];
	
	// News
//	NewsController *newsController = [[NewsController alloc] initWithFrame:applicationFrame];
//	newsController.managedObjectContext = self.managedObjectContext;
//	UINavigationController *newsNavigationController = [[UINavigationController alloc] initWithRootViewController:newsController];
//	[newsController release];
//
	// Settings
	SettingsTableViewController_Phone *settingsController = [[SettingsTableViewController_Phone alloc] init];
	UINavigationController *settingsNavigationController = [[UINavigationController alloc] initWithRootViewController:settingsController];
	[settingsController release];
	
//	NSArray *viewControllersArray = [NSArray arrayWithObjects:myListNavigationController, newsNavigationController, settingsNavigationController, nil];
	NSArray *viewControllersArray = [NSArray arrayWithObjects:myListNavigationController, settingsNavigationController, nil];

	[myListNavigationController release];
//	[newsNavigationController release];
	[settingsNavigationController release];
	
	self.tabController.viewControllers = viewControllersArray;
	
	_window = [[UIWindow alloc] initWithFrame:windowFrame];
	[_window addSubview:_tabController.view];
	
	_window.backgroundColor = [UIColor lightGrayColor];
	[_window makeKeyAndVisible];
	
	DataController *dataController = [DataController sharedManager];
	dataController.managedObjectContext = self.managedObjectContext;
	
	[[mTraderServerMonitor sharedManager] attemptConnection];
	
	return YES;
}


/**
 applicationWillTerminate: saves changes in the application's managed object context before the application terminates.
 */
- (void)applicationWillTerminate:(UIApplication *)application {
	[[UserDefaults sharedManager] saveSettings];
	
	NSError *error;
    if (_managedObjectContext != nil) {
        if ([_managedObjectContext hasChanges] && ![_managedObjectContext save:&error]) {
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
			abort();
#endif
        } 
    }
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

    _managedObjectModel = [[NSManagedObjectModel mergedModelFromBundles:nil] retain];    
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
	
	NSURL *storeUrl = [NSURL fileURLWithPath:[[self applicationDocumentsDirectory] stringByAppendingPathComponent:@"mTrader00.sqlite"]];
	
//	NSFileManager *fileManager = [NSFileManager defaultManager];
//	// If the expected store doesn't exist, copy the default store.
//	if (![fileManager fileExistsAtPath:storePath]) {
//		NSString *defaultStorePath = [[NSBundle mainBundle] pathForResource:@"mTrader" ofType:@"sqlite"];
//		if (defaultStorePath) {
//			[fileManager copyItemAtPath:defaultStorePath toPath:storePath error:NULL];
//		}
//	}
		
	NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
							 [NSNumber numberWithBool:YES], NSMigratePersistentStoresAutomaticallyOption,
							 [NSNumber numberWithBool:YES], NSInferMappingModelAutomaticallyOption, nil];	
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
/*
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
    NSLog(@"Queried about %@", NSStringFromSelector(sel));
    return [super respondsToSelector:sel];
}
*/

#pragma mark -
#pragma mark Memory managment

-(void) dealloc {	
	[_managedObjectContext release];
    [_managedObjectModel release];
    [_persistentStoreCoordinator release];
	
	[_tabController release];
	[_window release];
	
    [super dealloc];
}

@end
