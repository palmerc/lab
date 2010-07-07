//
//  PopupAppDelegate.h
//  Popup
//
//  Created by Cameron Lowell Palmer on 07.07.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class StatusController;

@interface PopupAppDelegate : NSObject <UIApplicationDelegate> {
	StatusController *_viewController;
	
    UIWindow *_window;
}

@end

