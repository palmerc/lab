//
//  iTraderAppDelegate.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright InFront AS 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
	MYSTOCKS = 1,
	NEWS = 2,
	SETTINGS = 3
} tabs;


@class MyStocksViewController;
@class NewsViewController;
@class SettingsTableViewController;
@class iTraderCommunicator;
@interface iTraderAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	UITabBarController *tabController;
	
	MyStocksViewController *myStocks;
	NewsViewController *news;
	UINavigationController *settingsNavigationController;
	
	NSUserDefaults *defaults;
	iTraderCommunicator *communicator;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) UITabBarController *tabController;

@end

