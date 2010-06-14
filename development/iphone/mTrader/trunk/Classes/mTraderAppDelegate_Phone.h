//
//  mTraderAppDelegate_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//


@class ChainsNavigationViewController;

typedef enum {
	CHAINS = 1,
	NEWS = 2,
	SETTINGS = 3
} tabs;

@interface mTraderAppDelegate_Phone : NSObject <UIApplicationDelegate, UINavigationControllerDelegate> {
@private
	NSManagedObjectModel *_managedObjectModel;
    NSManagedObjectContext *_managedObjectContext;
    NSPersistentStoreCoordinator *_persistentStoreCoordinator;
	
	UIWindow *_window;
	UITabBarController *_tabController;
}

@property (nonatomic, retain, readonly) NSManagedObjectModel *managedObjectModel;
@property (nonatomic, retain, readonly) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain, readonly) NSPersistentStoreCoordinator *persistentStoreCoordinator;

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) UITabBarController *tabController;

-(NSString *) applicationDocumentsDirectory;

@end

