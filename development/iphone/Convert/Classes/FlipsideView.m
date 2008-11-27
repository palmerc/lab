//
//  FlipsideView.m
//  Convert
//
//  Created by Cameron Palmer on 15/08/2008.
//  Copyright University of North Texas 2008. All rights reserved.
//

#import "FlipsideView.h"

@implementation FlipsideView


- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	return self;
}


- (void)drawRect:(CGRect)rect {
	/* Draw "Cameron Lowell Palmer" */
	NSString *hello = @"Cameron Lowell Palmer";
	CGPoint location = CGPointMake(10, 50);
	UIFont *font = [UIFont systemFontOfSize:24];
	[[UIColor whiteColor] set];
	[hello drawAtPoint:location withFont:font];
}


- (void)dealloc {
	[super dealloc];
}


@end
