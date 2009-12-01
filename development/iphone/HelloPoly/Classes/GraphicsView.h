//
//  PolyView.h
//  HelloPoly
//
//  Created by Cameron Lowell Palmer on 27.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface GraphicsView : UIView {
	int numberOfSides;
	float rotation;
	BOOL dashedStroke;
	
	IBOutlet UILabel *shapeName;
}

@property int numberOfSides;
@property float rotation;

- (void)updateShape:(NSString *)name numberOfSides:(int)sides dashedStroke:(BOOL)dashed;
- (NSArray *)pointsForPolygonInRect:(CGRect)rect;

@end
