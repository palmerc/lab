//
//  mTraderAppDelegate.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
@class LoginViewController;

@interface mTraderAppDelegate : NSObject <UIApplicationDelegate> {
	LoginViewController *loginViewController;
    UIWindow *window;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

