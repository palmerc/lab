//
//  WhatAToolAppDelegate.h
//  WhatATool
//
//  Created by Cameron Lowell Palmer on 19.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface WhatAToolAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
