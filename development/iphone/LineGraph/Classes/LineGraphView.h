//
//  LineGraphView.h
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 27.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface LineGraphView : UIView {
@private
	NSArray *points;
}

@property (nonatomic, retain) NSArray *points;

@end
