//
//  ConvertAppDelegate.h
//  Convert
//
//  Created by Cameron Palmer on 15/08/2008.
//  Copyright University of North Texas 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class RootViewController;

@interface ConvertAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet RootViewController *rootViewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *rootViewController;

@end

