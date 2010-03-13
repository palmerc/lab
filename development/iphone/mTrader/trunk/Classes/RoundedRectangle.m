//
//  RoundedRectangle.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 08.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangle.h"

@implementation RoundedRectangle
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
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	CGContextSetShadow(context, CGSizeMake(2,-2), kBlur);
	CGContextSetStrokeColorWithColor(context, [self.strokeColor CGColor]);
	CGContextSetFillColorWithColor(context, [self.rectColor CGColor]);
	CGContextSetLineWidth(context, self.strokeWidth);
	
	CGFloat leftPaddingReduction = self.padding;
	CGFloat topPaddingReduction = self.padding;
	CGFloat fw = rect.size.width - leftPaddingReduction - self.strokeWidth;
	CGFloat fh = rect.size.height - topPaddingReduction - self.strokeWidth;
	CGFloat zero = self.strokeWidth + leftPaddingReduction;

	CGContextMoveToPoint(context, fw, floor(fh/2));
	CGContextAddArcToPoint(context, fw, fh, floor(fw/2), fh, self.cornerRadius);
	CGContextAddArcToPoint(context, zero, fh, zero, floor(fh/2), self.cornerRadius);
	CGContextAddArcToPoint(context, zero, zero, floor(fw/2), zero, self.cornerRadius);
	CGContextAddArcToPoint(context, fw, zero, fw, floor(fh/2), self.cornerRadius);

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
