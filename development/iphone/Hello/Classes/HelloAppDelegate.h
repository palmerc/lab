//
//  HelloAppDelegate.h
//  Hello
//
//  Created by Cameron Palmer on 28/11/2008.
//  Copyright University of North Texas 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class HelloViewController;

@interface HelloAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    HelloViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet HelloViewController *viewController;

@end

