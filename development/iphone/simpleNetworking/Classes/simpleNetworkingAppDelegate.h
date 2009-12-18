//
//  simpleNetworkingAppDelegate.h
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
@class LoginViewController;

@interface simpleNetworkingAppDelegate : NSObject <UIApplicationDelegate> {
	LoginViewController *loginViewController;
    UIWindow *window;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

