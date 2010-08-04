//
//  WebViewAppDelegate.h
//  WebView
//
//  Created by Cameron Lowell Palmer on 04.08.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface WebViewAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	UINavigationController *nav;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

