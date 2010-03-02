//
//  LineGraphView.m
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 27.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


#import "LineGraphView.h"


@implementation LineGraphView
@synthesize points;

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        NSLog(@"View %f, %f, %f, %f", frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
	CGContextRef context = UIGraphicsGetCurrentContext(); //get the graphics context
	CGContextStrokePath(context);
	CGContextSetRGBStrokeColor(context, 1.0, 0.0, 0.0, 1.0); //there are two relevant color states, "Stroke" -- used in Stroke drawing functions and "Fill" - used in fill drawing functions
	//now we build a "path" 	you can either directly build it on the context or build a path object, here I build it on the context

	CGContextSetStrokeColorWithColor(context, [[UIColor colorWithWhite:1.0 alpha:1] CGColor]);
	CGContextSetLineWidth(context, 2);
	CGContextSetLineJoin(context, kCGLineJoinRound);
	CGContextSetLineCap(context, kCGLineCapRound);
	CGContextSetAllowsAntialiasing(context, YES);
	CGContextSetShouldAntialias(context, YES);
	// Begin Path
	CGPoint point = [[points objectAtIndex:0] CGPointValue];
	CGContextMoveToPoint(context, point.x, point.y);
	for (int i = 1; i < points.count; i++) {
		point = [[points objectAtIndex:i] CGPointValue];
		CGContextAddLineToPoint(context, point.x, point.y);
	}
	CGContextDrawPath(context, kCGPathFillStroke);

	CGContextSetStrokeColorWithColor(context, [[UIColor colorWithWhite:.7 alpha:0.7] CGColor]);
	for (int i = 0; i < points.count; i += 10) {
		context = UIGraphicsGetCurrentContext();
		point = [[points objectAtIndex:i] CGPointValue];
		CGContextMoveToPoint(context, point.x, point.y);
		CGContextAddLineToPoint(context, point.x, 480.0);
		CGContextAddLineToPoint(context, point.x, point.y);
		CGContextDrawPath(context, kCGPathFillStroke);

	}
}

#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"LineGraphView queried about %@", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [super dealloc];
}


@end
