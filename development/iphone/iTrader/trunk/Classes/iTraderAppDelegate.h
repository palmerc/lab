//
//  iTraderAppDelegate.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

typedef enum {
	MYSTOCKS = 1,
	NEWS = 2,
	SETTINGS = 3
} tabs;

@interface iTraderAppDelegate : NSObject <UIApplicationDelegate> {
	NSManagedObjectModel *managedObjectModel;
    NSManagedObjectContext *managedObjectContext;	    
    NSPersistentStoreCoordinator *persistentStoreCoordinator;
	
	UIWindow *window;
	UITabBarController *tabController;
	UINavigationController *myStocksNavigationController;
	UINavigationController *newsNavigationController;
	UINavigationController *settingsNavigationController;
	
	NSUserDefaults *defaults;
}

@property (nonatomic, retain, readonly) NSManagedObjectModel *managedObjectModel;
@property (nonatomic, retain, readonly) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain, readonly) NSPersistentStoreCoordinator *persistentStoreCoordinator;

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) UITabBarController *tabController;

-(NSString *) applicationDocumentsDirectory;

@end

