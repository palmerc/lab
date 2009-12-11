//
//  PresenceAppDelegate.h
//  Presence
//
//  Created by Cameron Lowell Palmer on 09.12.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PresenceAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	UINavigationController *controller;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

