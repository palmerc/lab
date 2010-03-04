//
//  mTraderAppDelegate.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

#import "mTraderAppDelegate.h"
#import "Starter.h"
#import "UserDefaults.h"
#import "ChainsTableViewController.h"
#import "ChainsNavigationViewController.h"
#import "NewsController.h"
#import "SettingsTableViewController.h"

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

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	// Start up the Singleton services
	Starter *starter = [[Starter alloc] init];
	[starter release];
		
	window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	//NSLog(@"Window size -> x:%.1f y:%.1f width:%.1f height:%.1f", window.frame.origin.x, window.frame.origin.y, window.frame.size.width, window.frame.size.height);
	window.backgroundColor = [UIColor lightGrayColor];
	
	UIViewController *rootViewController = [[ChainsTableViewController alloc] initWithManagedObjectContext:self.managedObjectContext];
	ChainsNavigationViewController *chainsNavigationController = [[ChainsNavigationViewController alloc] initWithContentViewController:rootViewController];
	[rootViewController release];
	
	NewsController *news = [[NewsController alloc] init];
	UINavigationController *newsNavigationController = [[UINavigationController alloc] initWithRootViewController:news];
	news.managedObjectContext = self.managedObjectContext;
	
	SettingsTableViewController *settings = [[SettingsTableViewController alloc] init];
	UINavigationController *settingsNavigationController = [[UINavigationController alloc] initWithRootViewController:settings];

	NSArray *viewControllersArray = [NSArray arrayWithObjects:chainsNavigationController, newsNavigationController, settingsNavigationController, nil];
		
	_tabController = [[UITabBarController alloc] init];
	//NSLog(@"TabController size -> x:%.1f y:%.1f width:%.1f height:%.1f", _tabController.view.frame.origin.x, _tabController.view.frame.origin.y, _tabController.view.frame.size.width, _tabController.view.frame.size.height);
	//NSLog(@"TabController size -> x:%.1f y:%.1f width:%.1f height:%.1f", _tabController.view.bounds.origin.x, _tabController.view.bounds.origin.y, _tabController.view.bounds.size.width, _tabController.view.bounds.size.height);
	self.tabController.viewControllers = viewControllersArray;
	 	
	[window addSubview:self.tabController.view];
	[window makeKeyAndVisible];
	
	[news release];
	[settings release];
	
	[chainsNavigationController release];
	[newsNavigationController release];
	[settingsNavigationController release];
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
	
	
	NSString *storePath = [[self applicationDocumentsDirectory] stringByAppendingPathComponent: @"mTrader.sqlite"];
	/*
	 Set up the store.
	 For the sake of illustration, provide a pre-populated default store.
	 */
	NSFileManager *fileManager = [NSFileManager defaultManager];
	// If the expected store doesn't exist, copy the default store.
	if (![fileManager fileExistsAtPath:storePath]) {
		NSString *defaultStorePath = [[NSBundle mainBundle] pathForResource:@"mTrader" ofType:@"sqlite"];
		if (defaultStorePath) {
			[fileManager copyItemAtPath:defaultStorePath toPath:storePath error:NULL];
		}
	}
	
	NSURL *storeUrl = [NSURL fileURLWithPath:storePath];
	
	NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES], NSMigratePersistentStoresAutomaticallyOption, [NSNumber numberWithBool:YES], NSInferMappingModelAutomaticallyOption, nil];	
    persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel: [self managedObjectModel]];
	
	NSError *error;
	if (![persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeUrl options:options error:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
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
	[window release];
    [super dealloc];
}


@end
