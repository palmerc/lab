//
//  TableDemoAppDelegate.h
//  TableDemo
//
//  Created by Cameron Lowell Palmer on 07.12.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TableViewController;

@interface TableDemoAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	TableViewController *aTableViewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

