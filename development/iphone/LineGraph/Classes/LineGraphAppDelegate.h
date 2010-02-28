//
//  LineGraphAppDelegate.h
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 27.02.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//


@class LineGraphController;

@interface LineGraphAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	
	LineGraphController *controller;
}

@property (nonatomic, retain) UIWindow *window;

@end

