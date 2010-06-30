//
//  mTraderAppDelegate.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

#define DEBUG 0

#import "mTraderAppDelegate.h"

#import "mTraderServerMonitor.h"
#import "Reachability.h"
#import "Starter.h"
#import "UserDefaults.h"
#import "MyListViewController.h"
#import "MyListNavigationController.h"
#import "NewsTableViewController_Phone.h"
#import "SettingsTableViewController.h"
#import "SymbolDataController.h"

@implementation mTraderAppDelegate
@synthesize window;
@synthesize tabController = _tabController;

// +initialize is invoked before the class receives any other messages, so it
// is a good place to set up application defaults
+ (void)initialize {
    if ([self class] == [mTraderAppDelegate class]) {
		NSArray *keys = [NSArray arrayWithObjects:@"username", @"password", nil];
		NSArray *values = [NSArray arrayWithObjects:@"", @"", nil];
		
		NSDictionary *resourceDict = [NSDictionary dictionaryWithObjects:values	forKeys:keys];
		[[NSUserDefaults standardUserDefaults] registerDefaults:resourceDict];
    }
}

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	CGRect applicationFrame = [[UIScreen mainScreen] applicationFrame];

	// Start up the Singleton services
	Starter *starter = [[Starter alloc] initWithManagedObjectContext:self.managedObjectContext];
	[starter release];
		
	window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	window.backgroundColor = [UIColor lightGrayColor];
	
	UIViewController *rootViewController = [[MyListViewController alloc] initWithManagedObjectContext:self.managedObjectContext];
	MyListNavigationController *myListNavigationController = [[MyListNavigationController alloc] initWithContentViewController:rootViewController];
	[rootViewController release];
	
	NewsTableViewContoller_Phone *news = [[NewsTableViewContoller_Phone alloc] initWithFrame:applicationFrame];
	news.managedObjectContext = self.managedObjectContext;
	UINavigationController *newsNavigationController = [[UINavigationController alloc] initWithRootViewController:news];
	news.managedObjectContext = self.managedObjectContext;
	
	SettingsTableViewController *settings = [[SettingsTableViewController alloc] init];
	UINavigationController *settingsNavigationController = [[UINavigationController alloc] initWithRootViewController:settings];

	NSArray *viewControllersArray = [NSArray arrayWithObjects:myListNavigationController, newsNavigationController, settingsNavigationController, nil];
		
	_tabController = [[UITabBarController alloc] init];
	self.tabController.viewControllers = viewControllersArray;
	 	
	[window addSubview:self.tabController.view];
	[window makeKeyAndVisible];
	
	[news release];
	[settings release];
	
	[myListNavigationController release];
	[newsNavigationController release];
	[settingsNavigationController release];
	
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
	[[mTraderServerMonitor sharedManager] logout];

	[[UserDefaults sharedManager] saveSettings];
	
	NSError *error;
    if (managedObjectContext != nil) {
        if ([managedObjectContext hasChanges] && ![managedObjectContext save:&error]) {
			// Update to handle the error appropriately.
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
			abort();
#endif
        } 
    }
}

//- (void)applicationWillEnterForeground:(UIApplication *)application {
//	[[mTraderServerMonitor sharedManager] attemptConnection];
//}

/**
 applicationWillTerminate: saves changes in the application's managed object context before the application terminates.
 */
- (void)applicationWillTerminate:(UIApplication *)application {
	[[mTraderServerMonitor sharedManager] logout];

	[[UserDefaults sharedManager] saveSettings];
	
	NSError *error;
    if (managedObjectContext != nil) {
        if ([managedObjectContext hasChanges] && ![managedObjectContext save:&error]) {
			// Update to handle the error appropriately.
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
			abort();
#endif
        } 
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	[[mTraderServerMonitor sharedManager] attemptConnection];
}



#pragma mark -
#pragma mark Core Data stack

/**
 Returns the managed object context for the application.
 If the context doesn't already exist, it is created and bound to the persistent store coordinator for the application.
 */
- (NSManagedObjectContext *) managedObjectContext {
	
    if (managedObjectContext != nil) {
        return managedObjectContext;
    }
	
    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
    if (coordinator != nil) {
        managedObjectContext = [[NSManagedObjectContext alloc] init];
        [managedObjectContext setPersistentStoreCoordinator: coordinator];
    }
    return managedObjectContext;
}


/**
 Returns the managed object model for the application.
 If the model doesn't already exist, it is created by merging all of the models found in the application bundle.
 */
- (NSManagedObjectModel *)managedObjectModel {
    if (managedObjectModel != nil) {
        return managedObjectModel;
    }
	
	NSString *modelPath = [[NSBundle mainBundle] pathForResource:@"Symbols" ofType:@"momd"];
	NSAssert(modelPath != nil, @"Managed Object Model is not found.");
	NSURL *modelURL = [NSURL fileURLWithPath:modelPath];
    managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];    
    
	return managedObjectModel;
}


/**
 Returns the persistent store coordinator for the application.
 If the coordinator doesn't already exist, it is created and the application's store added to it.
 */
- (NSPersistentStoreCoordinator *)persistentStoreCoordinator {
	
    if (persistentStoreCoordinator != nil) {
        return persistentStoreCoordinator;
    }
		NSString *storeType = NSSQLiteStoreType;
	NSString *storePath = [[self applicationDocumentsDirectory] stringByAppendingPathComponent: @"mTrader01.sqlite"];
	NSURL *storeURL = [NSURL fileURLWithPath:storePath];

	persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];
	NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES], NSMigratePersistentStoresAutomaticallyOption, nil];	
	
	NSError *error = nil;
	if (![persistentStoreCoordinator addPersistentStoreWithType:storeType configuration:nil URL:storeURL options:options error:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();  // Fail
#endif
    }    
	
    return persistentStoreCoordinator;
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
    NSLog(@"Queried about %@", NSStringFromSelector(sel));
    return [super respondsToSelector:sel];
}
#endif

#pragma mark -
#pragma mark Memory managment

-(void) dealloc {	
	[managedObjectContext release];
    [managedObjectModel release];
    [persistentStoreCoordinator release];
	
	[_tabController release];
	[window release];
    [super dealloc];
}


@end
