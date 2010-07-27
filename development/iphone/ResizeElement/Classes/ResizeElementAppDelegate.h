//
//  ResizeElementAppDelegate.h
//  ResizeElement
//
//  Created by Cameron Lowell Palmer on 27.07.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//


@class CPView;

@interface ResizeElementAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	CPView *cpView;

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

