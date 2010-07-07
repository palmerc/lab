//
//  StatusView.m
//  Popup
//
//  Created by Cameron Lowell Palmer on 07.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "StatusView.h"


@implementation StatusView
@synthesize message = _message;
@synthesize activityIndicator = _activityIndicator;

#define PADDING 10.0f

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		_message = nil;
		
		CGRect bounds = self.bounds;
		
		CGFloat height = 20.0f;
		CGFloat center = bounds.size.height / 2.0f;
		CGRect activityFrame = CGRectMake(PADDING, center - 10.0f, 20.0f, height);
		
		CGFloat x = activityFrame.origin.x + activityFrame.size.width + PADDING; 
		CGFloat width = bounds.size.width - activityFrame.size.width - 3 * PADDING;
		CGRect statusFrame = CGRectMake(x, center - 10.0f, width, height);
				
		_activityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
		_activityIndicator.frame = activityFrame;
		
		[self addSubview:_activityIndicator];
		
		[_activityIndicator release];
    }
    return self;
}

#define STRING_INDENT 20

- (void)drawRect:(CGRect)rect {
	
	// Draw the placard at 0, 0
	//[placardImage drawAtPoint:(CGPointMake(0.0, 0.0))];
	
	/*
	 Draw the current display string.
	 Typically you would use a UILabel, but this example serves to illustrate the UIKit extensions to NSString.
	 The text is drawn center of the view twice - first slightly offset in black, then in white -- to give an embossed appearance.
	 The size of the font and text are calculated in setupNextDisplayString.
	 */
	
	// Find point at which to draw the string so it will be in the center of the view
	CGFloat x = self.bounds.size.width/2 - textSize.width/2;
	CGFloat y = self.bounds.size.height/2 - textSize.height/2;
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
