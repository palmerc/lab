//
//  StatusView.m
//  Popup
//
//  Created by Cameron Lowell Palmer on 07.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "StatusView.h"


@implementation StatusView
@synthesize message = _message;
@synthesize activityIndicator = _activityIndicator;

#define PADDING 10.0f

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		super.padding = 6.0f;
		super.cornerRadius = 10.0f;
		super.strokeWidth = 2.0f;
		_message = nil;
		
		CGRect bounds = self.bounds;
		
		CGFloat height = 20.0f;
		CGFloat center = bounds.size.height / 2.0f;
		CGRect activityFrame = CGRectMake(PADDING, center - 10.0f, 20.0f, height);
				
		_activityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
		_activityIndicator.frame = activityFrame;
		
		[self addSubview:_activityIndicator];
		
		[_activityIndicator release];
    }
    return self;
}

#define STRING_INDENT 20

- (void)drawRect:(CGRect)rect {
	[super drawRect:rect];
	//[placardImage drawAtPoint:(CGPointMake(0.0, 0.0))];
	
	// Find point at which to draw the string so it will be in the center of the view
	CGFloat x = self.bounds.size.width / 2.0f - textSize.width / 2.0f;
	CGFloat y = self.bounds.size.height / 2.0f - textSize.height / 2.0f;
	CGPoint point;
	
	// Get the font of the appropriate size
	UIFont *font = [UIFont systemFontOfSize:fontSize];
	
	[[UIColor blackColor] set];
	point = CGPointMake(x, y + 0.5);
	[_message drawAtPoint:point forWidth:(self.bounds.size.width - STRING_INDENT) withFont:font fontSize:fontSize lineBreakMode:UILineBreakModeMiddleTruncation baselineAdjustment:UIBaselineAdjustmentAlignBaselines];
	
	[[UIColor whiteColor] set];
	point = CGPointMake(x, y);
	[_message drawAtPoint:point forWidth:(self.bounds.size.width - STRING_INDENT) withFont:font fontSize:fontSize lineBreakMode:UILineBreakModeMiddleTruncation baselineAdjustment:UIBaselineAdjustmentAlignBaselines]; 
}

- (void)setMessage:(NSString *)message {
	if (message != _message) {
		[_message release];
		_message = [message retain];
	}
	
	UIFont *font = [UIFont systemFontOfSize:24];
	// Precalculate size of text and size of font so that text fits inside placard
	textSize = [_message sizeWithFont:font minFontSize:9.0 actualFontSize:&fontSize forWidth:(self.bounds.size.width - STRING_INDENT) lineBreakMode:UILineBreakModeMiddleTruncation];
	[self setNeedsDisplay];
}

- (void)dealloc {
	[_message release];
	
    [super dealloc];
}


@end
