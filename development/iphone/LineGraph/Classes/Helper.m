//
//  Helper.m
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 28.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Helper.h"

#import "DataPoint.h"

@implementation Helper

+ (NSArray *)graphPointsFromDataPoints:(NSArray *)dataPoints {
	NSDateFormatter *dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
	dateFormatter.timeStyle = NSDateFormatterShortStyle;
		
	NSMutableArray *graphPoints = [[[NSMutableArray alloc] init] autorelease];
	CGFloat xScalingFactor = [dataPoints count] / 320.0;
	
	int i = 0;
	for (DataPoint *dataPoint in dataPoints) {
		//NSDate *date = dataPoint.timestamp;
		NSNumber *number = dataPoint.close;
		
		CGPoint point;
		point.x = i / xScalingFactor;
		point.y = [number doubleValue] / 32.25;
		NSValue *value = [NSValue valueWithCGPoint:point];
		[graphPoints addObject:value];
		
		i++;
	}
	
	return graphPoints;
}

@end
