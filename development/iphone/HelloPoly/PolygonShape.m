//
//  PolygonShape.m
//  WhatATool
//
//  Created by Cameron Lowell Palmer on 24.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "PolygonShape.h"


@implementation PolygonShape

@synthesize numberOfSides;
@synthesize minimumNumberOfSides;
@synthesize maximumNumberOfSides;

- (id)init {
	return [self initWithNumberOfSides:5 minimumNumberOfSides:3 maximumNumberOfSides:12];
}

- (id)initWithNumberOfSides:(int)sides minimumNumberOfSides:(int)min maximumNumberOfSides:(int)max {
	self = [super init];
	
	if (self) {
		[self setMinimumNumberOfSides:min];
		[self setMaximumNumberOfSides:max];
		[self setNumberOfSides:sides];
	}
	return self;
}

- (void)dealloc {
	NSLog(@"Calling dealloc on %@", self.name);
	
	[super dealloc];
}

- (void)setMinimumNumberOfSides:(int)min {
	if (! (min > 2 && min <= 12)) {
		NSLog(@"Invalid number of sides: %d must be greater than 2", min);
	} else {
		minimumNumberOfSides = min;
	}
}

- (void)setMaximumNumberOfSides:(int)max {
	if (! (max > 2 && max <= 12)) {
		NSLog(@"Invalid number of sides: %d must be less than or equal to 12", max);
	} else {
		maximumNumberOfSides = max;
	}
}

- (void)setNumberOfSides:(int)sides {
	if ( sides < minimumNumberOfSides ) {
		NSLog(@"Invalid number of sides: %d must be greater than or equal to %d", sides, minimumNumberOfSides);
	} else if ( sides > maximumNumberOfSides ) {
		NSLog(@"Invalid number of sides: %d must be less than or equal to %d", sides, maximumNumberOfSides);
	} else {
		numberOfSides = sides;
	}
}

- (NSString *)name {
	switch (numberOfSides) {
		case 3:
			return @"Triangle";
			break;
		case 4:
			return @"Quadrilateral";
			break;
		case 5:
			return @"Pentagon";
			break;
		case 6:
			return @"Hexagon";
			break;
		case 7:
			return @"Heptagon";
			break;
		case 8:
			return @"Octagon";
			break;
		case 9:
			return @"Enneagon";
			break;
		case 10:
			return @"Decagon";
			break;
		case 11:
			return @"Hendecagon";
			break;
		case 12:
			return @"Dodecagon";
			break;
		default:
			return @"Invalid";
			break;
	}

}

- (NSString *)description {
	return [[NSString alloc] initWithFormat:@"Hello. I am a %d-sided polygon (AKA a %@) with angles of %f degrees (%f radians).", 
		  numberOfSides, self.name, self.angleInDegrees, self.angleInRadians];
}

- (float)angleInDegrees {
	return 180 * (numberOfSides - 2) / numberOfSides;
}

- (float)angleInRadians {
	return M_PI * (numberOfSides - 2) / numberOfSides;
}

@end
