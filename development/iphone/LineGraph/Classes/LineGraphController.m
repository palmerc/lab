//
//  LineGraphController.m
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 27.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


#import "LineGraphController.h"

#import "LineGraphView.h"

@implementation LineGraphController
@synthesize points;

- (id)initWithFrame:(CGRect)_frame {
	self = [super init];
	if (self != nil) {
		frame = _frame;
		NSLog(@"LineGraphController: %f, %f, %f, %f", frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
	}
	return self;
}

- (void)loadView {
	[super loadView];
	
	lineGraphView = [[LineGraphView alloc] initWithFrame:frame];
	lineGraphView.points = points;
	[self.view addSubview:lineGraphView];
}



#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"LineGraphController queried about %@", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}


#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[lineGraphView release];
	
    [super dealloc];
}


@end

