//
//  PolygonShape.h
//  WhatATool
//
//  Created by Cameron Lowell Palmer on 24.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PolygonShape : NSObject {
	int numberOfSides;
	int minimumNumberOfSides;
	int maximumNumberOfSides;
	float angleInDegrees;
	float angleInRadians;
}

@property (readonly) NSString *name;
@property int numberOfSides;
@property int minimumNumberOfSides;
@property int maximumNumberOfSides;
@property (readonly) float angleInDegrees;
@property (readonly) float angleInRadians;

- (id)initWithNumberOfSides:(int)sides minimumNumberOfSides:(int)min maximumNumberOfSides:(int)max;
- (NSString *)name;

@end
