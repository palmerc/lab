//
//  RoundedRectangle.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 08.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangle.h"

#import "RoundedRectangleFrame.h"

@implementation RoundedRectangle

- (id)initWithFrame:(CGRect)frame {
	CGFloat padding = 6.0f;
	CGRect roundedFrame = frame;
	CGRect viewFrame = CGRectMake(padding, padding, frame.size.width - 2.0f * padding, frame.size.height - 2.0f * padding);

	self = [super initWithFrame:viewFrame];
	if (self != nil) {
		self.backgroundColor = [UIColor yellowColor];
		_rrf = [[RoundedRectangleFrame alloc] initWithFrame:roundedFrame];
		_rrf.padding = 6.0f;
		_rrf.cornerRadius = 10.0f;
		_rrf.strokeWidth = 0.75f;
		[_rrf addSubview:self];		
	}	
	return self;
}

- (void)dealloc {
	[_rrf release];
	
	[super dealloc];
}

@end