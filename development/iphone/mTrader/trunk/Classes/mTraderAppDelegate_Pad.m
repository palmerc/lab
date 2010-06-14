//
//  mTraderAppDelegate_Pad.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

#import "mTraderAppDelegate_Pad.h"

#import "Reachability.h"
#import "Starter.h"
#import "UserDefaults.h"
#import "MyListHeaderViewController_Phone.h"
#import "MyListNavigationController_Phone.h"
#import "NewsController.h"
#import "SettingsTableViewController.h"

@implementation mTraderAppDelegate_Pad
@synthesize window = _window;
@synthesize tabController = _tabController;

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

	managedObjectContext = nil;
	managedObjectModel = nil;
	persistentStoreCoordinator = nil;
	
	// Start up the Singleton services
	Starter *starter = [[Starter alloc] initWithManagedObjectContext:self.managedObjectContext];
	[starter release];

	MyListHeaderViewController_Phone *rootViewController = [[MyListHeaderViewController_Phone alloc] initWithFrame:applicationFrame];
	rootViewController.managedObjectContext = self.managedObjectContext;
	MyListNavigationController_Phone *myListNavigationController = [[MyListNavigationController_Phone alloc] initWithContentViewController:(UIViewController *)rootViewController];
	[rootViewController release];
	
	NewsController *news = [[NewsController alloc] initWithMangagedObjectContext:self.managedObjectContext];
	UINavigationController *newsNavigationController = [[UINavigationController alloc] initWithRootViewController:news];
	[news release];

	SettingsTableViewController *settings = [[SettingsTableViewController alloc] init];
	UINavigationController *settingsNavigationController = [[UINavigationController alloc] initWithRootViewController:settings];
	[settings release];
	
	NSArray *viewControllersArray = [NSArray arrayWithObjects:myListNavigationController, newsNavigationController, settingsNavigationController, nil];
	[myListNavigationController release];
	[newsNavigationController release];
	[settingsNavigationController release];
	
	_tabController = [[UITabBarController alloc] init];
	self.tabController.viewControllers = viewControllersArray;
	
	_window = [[UIWindow alloc] initWithFrame:windowFrame];
	[_window addSubview:_tabController.view];
	
	_window.backgroundColor = [UIColor lightGrayColor];
	[_window makeKeyAndVisible];
	
	return YES;
}


/**
 applicationWillTerminate: saves changes in the application's managed object context before the application terminates.
 */
- (void)applicationWillTerminate:(UIApplication *)application {
	[[UserDefaults sharedManager] saveSettings];
	
	NSError *error;
    if (managedObjectContext != nil) {
        if ([managedObjectContext hasChanges] && ![managedObjectContext save:&error]) {
			// Update to handle the error appropriately.
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
			abort();
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
    if (managedObjectContext != nil) {
        return managedObjectContext;
    }
	
    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
    if (coordinator != nil) {
        managedObjectContext = [[NSManagedObjectContext alloc] init];
        [managedObjectContext setPersistentStoreCoordinator:coordinator];
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

    managedObjectModel = [[NSManagedObjectModel mergedModelFromBundles:nil] retain];    
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
    persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];
	
	NSError *error;
	if (![persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeUrl options:options error:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Could not start persistent store: %@, %@", error, [error userInfo]);
		abort();  // Fail
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
	[managedObjectContext release];
    [managedObjectModel release];
    [persistentStoreCoordinator release];
	
	[_tabController release];
	[_window release];
	
    [super dealloc];
}


@end
