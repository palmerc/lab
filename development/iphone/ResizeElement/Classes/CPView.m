//
//  CPView.m
//  ResizeElement
//
//  Created by Cameron Lowell Palmer on 27.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


#import "CPView.h"

@implementation CPView

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		topBar = [[UIView alloc] initWithFrame:CGRectZero];
		topBar.backgroundColor = [UIColor yellowColor];
		[self addSubview:topBar];
		
		bottomBar = [[UIView alloc] initWithFrame:CGRectZero];
		bottomBar.backgroundColor = [UIColor redColor];
		[self addSubview:bottomBar];
	}
    return self;
}

- (void)layoutSubviews {
	CGRect topBarFrame = self.bounds;
	topBarFrame.size.height = 50.0f;
	topBar.frame = topBarFrame;
	
	CGRect bottomBarFrame = self.bounds;
	bottomBarFrame.size.height = 50.0f;
	bottomBarFrame.origin.y = self.bounds.size.height - 50.0f;
	bottomBar.frame = bottomBarFrame;
}

- (void)dealloc {
	[topBar release];
	[bottomBar release];
	
    [super dealloc];
}


@end
