//
//  LineGraphController.h
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 27.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


@class LineGraphView;

@interface LineGraphController : UIViewController {
@private
	CGRect frame;
	LineGraphView *lineGraphView;
		
	NSArray *points;
}

@property (nonatomic, retain) NSArray *points;

@end
