//
//  NetworkDiagnosticAppDelegate.h
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 09.04.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NetworkDiagnosticViewController;

@interface NetworkDiagnosticAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *_window;
    NetworkDiagnosticViewController *_viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) NetworkDiagnosticViewController *viewController;

@end

