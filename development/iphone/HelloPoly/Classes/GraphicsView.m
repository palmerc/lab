//
//  PolyView.m
//  HelloPoly
//
//  Created by Cameron Lowell Palmer on 27.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "GraphicsView.h"
#import <CoreGraphics/CoreGraphics.h>

double square(double x) {
	return x * x;
}

@implementation GraphicsView

@synthesize numberOfSides;
@synthesize rotation;

- (void)awakeFromNib {
	//self.backgroundColor = [UIColor grayColor];
}

- (void)drawRect:(CGRect)rect {
	CGContextRef context = UIGraphicsGetCurrentContext();
	
    size_t num_locations = 2;
    CGFloat locations[2] = { 0.0, 1.0 };
    CGFloat components[4] = { 0.25, 0.25, 0.5, 0.75 }; // End color
	CGColorSpaceRef rgbColorspace = CGColorSpaceCreateDeviceRGB();
    CGGradientRef gradient = CGGradientCreateWithColorComponents(rgbColorspace, components, locations, num_locations);
	
    CGRect currentBounds = self.bounds;
    CGPoint topCenter = CGPointMake(CGRectGetMinX(currentBounds), 0.0f);
    CGPoint bottomRight = CGPointMake(CGRectGetMaxX(currentBounds), CGRectGetMaxY(currentBounds));
    CGContextDrawLinearGradient(context, gradient, topCenter, bottomRight, 0);
	
    CGGradientRelease(gradient);
    CGColorSpaceRelease(rgbColorspace);
	
	CGContextBeginPath(context);
	CGRect frame = self.bounds;	
	NSArray *points = [self pointsForPolygonInRect:frame];
		
	CGPoint point = [[points objectAtIndex:0] CGPointValue];
	CGContextMoveToPoint(context, point.x, point.y);
	for (int i = 1; i < points.count; i++) {
		point = [[points objectAtIndex:i] CGPointValue];
		CGContextAddLineToPoint(context, point.x, point.y);
	}
	CGContextClosePath(context);
	if (dashedStroke) {
		CGFloat lengths[] = {5, 5};
		CGContextSetLineDash(context, 0, lengths, 2);
	}
	[[UIColor whiteColor] setFill];
	[[UIColor blackColor] setStroke];
	CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)updateShape:(NSString *)name numberOfSides:(int)sides dashedStroke:(BOOL)dashed {
	dashedStroke = dashed;
	numberOfSides = sides;
	[self setNeedsDisplay];
	shapeName.text = name;
}

- (NSArray *)pointsForPolygonInRect:(CGRect)rect {
	CGPoint center = CGPointMake(rect.size.width / 2.0, rect.size.height / 2.0);
	float radius = 0.9 * center.x;
	NSMutableArray *result = [NSMutableArray array];
	float angle = (2.0 * M_PI) / numberOfSides;
	float exteriorAngle = M_PI - angle;
	float rotationDelta = angle - (0.5 * exteriorAngle) + self.rotation;
	for (int currentAngle = 0; currentAngle < numberOfSides; currentAngle++) {
		float newAngle = (angle * currentAngle) - rotationDelta;
		float curX = cos(newAngle) * radius;
		float curY = sin(newAngle) * radius;
		CGPoint point = CGPointMake(center.x + curX, center.y + curY);
		[result addObject:[NSValue valueWithCGPoint:point]];
	} 
	return result;
}

- (void)dealloc {
	[super dealloc];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	UITouch *touch = [touches anyObject];
	CGRect currentBounds = self.bounds;
	
	CGFloat halfX = CGRectGetMaxX(currentBounds) / 2.0;
	CGFloat halfY = CGRectGetMaxY(currentBounds) / 2.0;
	
	CGPoint previousTouch = [touch previousLocationInView:self];
	CGPoint currentTouch = [touch locationInView:self];
	
	CGPoint previousLocation = CGPointMake(previousTouch.x - halfX, previousTouch.y - halfY);
	CGPoint currentLocation = CGPointMake(currentTouch.x - halfX, currentTouch.y - halfY);
	double is = previousLocation.x * currentLocation.x;
	double js = previousLocation.y * currentLocation.y;
	double vDotW = is + js;
	double magnitudeV = sqrt( square(previousLocation.x) + square(previousLocation.y) );
	double magnitudeW = sqrt( square(currentLocation.x) + square(currentLocation.y) );
	float radians = acos(vDotW / (magnitudeV * magnitudeW));
	float sign = currentLocation.x * previousLocation.y -  previousLocation.x * currentLocation.y;
	
	self.rotation += radians * sign/abs(sign);
	[self setNeedsDisplay];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
}



@end
