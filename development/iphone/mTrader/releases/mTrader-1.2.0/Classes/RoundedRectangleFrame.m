	//
//  RoundedRectangleFrame.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 08.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangleFrame.h"

@implementation RoundedRectangleFrame
@synthesize strokeColor = _strokeColor;
@synthesize rectColor = _rectColor;
@synthesize strokeWidth = _strokeWidth;
@synthesize cornerRadius = _cornerRadius;
@synthesize padding = _padding;

#pragma mark -
#pragma mark Initialization

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
		self.opaque = NO;
		self.backgroundColor = [UIColor clearColor];
		
        _strokeColor = [[UIColor darkGrayColor] retain];
        _rectColor = [[UIColor whiteColor] retain];
        _strokeWidth = 0.0f;
        _cornerRadius = 0.0f;
		_padding = 0.0f;
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	CGContextSetShadow(context, CGSizeMake(2,-2), kBlur);
	CGContextSetStrokeColorWithColor(context, [_strokeColor CGColor]);
	CGContextSetFillColorWithColor(context, [_rectColor CGColor]);
	CGContextSetLineWidth(context, _strokeWidth);
	
	CGFloat leftPaddingReduction = _padding;
	CGFloat topPaddingReduction = _padding;
	CGFloat fw = rect.size.width - leftPaddingReduction - _strokeWidth;
	CGFloat fh = rect.size.height - topPaddingReduction - _strokeWidth;
	CGFloat zero = _strokeWidth + leftPaddingReduction;
	
	CGContextMoveToPoint(context, fw, floor(fh/2));
	CGContextAddArcToPoint(context, fw, fh, floor(fw/2), fh, _cornerRadius);
	CGContextAddArcToPoint(context, zero, fh, zero, floor(fh/2), _cornerRadius);
	CGContextAddArcToPoint(context, zero, zero, floor(fw/2), zero, _cornerRadius);
	CGContextAddArcToPoint(context, fw, zero, fw, floor(fh/2), _cornerRadius);
	
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
