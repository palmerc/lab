//
//  BBLinksAppDelegate.h
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

@class BBLinksViewController;

@interface BBLinksAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    BBLinksViewController *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) BBLinksViewController *viewController;

@end

