//
//  HalfRoundedRectangle.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 08.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define kBlur 3.0

#import "HalfRoundedRectangle.h"

@implementation HalfRoundedRectangle
@synthesize strokeColor = _strokeColor;
@synthesize rectColor = _rectColor;
@synthesize strokeWidth;
@synthesize cornerRadius;
@synthesize padding;

#pragma mark -
#pragma mark Initialization

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
		self.opaque = NO;
		self.backgroundColor = [UIColor clearColor];

        self.strokeColor = [UIColor darkGrayColor];
        self.rectColor = [UIColor whiteColor];
        self.strokeWidth = 0.0f;
        self.cornerRadius = 0.0f;
		self.padding = 0.0f;
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	[super drawRect:rect];
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	CGContextSetShadow(context, CGSizeMake(2,-2), kBlur);
	CGContextSetStrokeColorWithColor(context, [self.strokeColor CGColor]);
	CGContextSetFillColorWithColor(context, [self.rectColor CGColor]);
	CGContextSetLineWidth(context, self.strokeWidth);
	
	CGFloat fw = rect.size.width - kBlur;
	CGFloat fh = rect.size.height - kBlur;
	CGFloat zeroX = kBlur;
	CGFloat zeroY = kBlur;

	// x1, y1, x2, y2
	CGContextMoveToPoint(context, fw, floorf(fh/2)); // middle-right
	// x1, y1, x2, y2, radius
	CGContextAddArcToPoint(context, fw, fh, floorf(fw/2.0f), fh, 0.0f); // bottom-right to bottom-center
	CGContextAddArcToPoint(context, zeroX, fh, zeroX, floorf(fh/2.0f), 0.0f); // bottom-left to middle-left
	CGContextAddArcToPoint(context, zeroX, zeroY, floorf(fw/2.0f), zeroY, self.cornerRadius); // top-left to top-center
	CGContextAddArcToPoint(context, fw, zeroY, fw, floorf(fh/2.0f), self.cornerRadius); // top-right to middle-right

	CGContextClosePath(context);
	CGContextDrawPath(context, kCGPathFillStroke);
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_strokeColor release];
	[_rectColor release];
	
    [super dealloc];
}


@end
