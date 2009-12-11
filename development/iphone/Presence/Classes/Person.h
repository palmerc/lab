//
//  Person.h
//  Presence
//
//  Created by Cameron Lowell Palmer on 10.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface Person : NSObject {
	NSString *name;
	NSString *status;
	UIImage *image;
}

@property (retain) NSString *name;
@property (retain) NSString *status;
@property (retain) UIImage *image;

+ (NSArray *)initWithNames:(NSString *)firstObj, ... NS_REQUIRES_NIL_TERMINATION;

@end
