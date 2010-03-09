//
//  RoundedRectangle.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 08.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangle.h"

@implementation RoundedRectangle
@synthesize strokeColor;
@synthesize rectColor;
@synthesize strokeWidth;
@synthesize cornerRadius;

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
		self.opaque = NO;
        self.strokeColor = kDefaultStrokeColor;
        self.backgroundColor = [UIColor clearColor];
        self.rectColor = kDefaultRectColor;
        self.strokeWidth = kDefaultStrokeWidth;
        self.cornerRadius = kDefaultCornerRadius;
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	CGContextSetShadow(context, CGSizeMake(2,-2), 3);
	CGContextSetStrokeColorWithColor(context, [self.strokeColor CGColor]);
	CGContextSetFillColorWithColor(context, [self.rectColor CGColor]);
	CGContextSetLineWidth(context, strokeWidth);
	
	NSLog(@"%f %f %f %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	CGFloat fw = rect.size.width - 4.5 - strokeWidth;
	CGFloat fh = rect.size.height - 4.5 - strokeWidth;
	CGFloat zero = strokeWidth + 6.0;

	CGContextMoveToPoint(context, fw, floor(fh/2));
	CGContextAddArcToPoint(context, fw, fh, floor(fw/2), fh, cornerRadius);
	CGContextAddArcToPoint(context, zero, fh, zero, floor(fh/2), cornerRadius);
	CGContextAddArcToPoint(context, zero, zero, floor(fw/2), zero, cornerRadius);
	CGContextAddArcToPoint(context, fw, zero, fw, floor(fh/2), cornerRadius);

	CGContextClosePath(context);
	CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)dealloc {
    [super dealloc];
}


@end
