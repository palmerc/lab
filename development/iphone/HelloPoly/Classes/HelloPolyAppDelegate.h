//
//  HelloPolyAppDelegate.h
//  HelloPoly
//
//  Created by Cameron Lowell Palmer on 26.11.09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>


@class MainViewController;

@interface HelloPolyAppDelegate : NSObject <UIApplicationDelegate> {
	UIWindow *window;
	IBOutlet MainViewController *mainViewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MainViewController *mainViewController;

@end

