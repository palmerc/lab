//
//  GradientAppDelegate.h
//  Gradient
//
//  Created by Cameron Lowell Palmer on 22.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GradientViewController;

@interface GradientAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    GradientViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet GradientViewController *viewController;

@end

