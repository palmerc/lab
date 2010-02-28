//
//  Point.h
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 28.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


@interface DataPoint : NSObject {
	NSDate *timestamp;
	NSNumber *close;
	NSNumber *high;
	NSNumber *low; 
	NSNumber *open;
	NSNumber *volume;
}

@property (nonatomic, retain) NSDate *timestamp;
@property (nonatomic, retain) NSNumber *close;
@property (nonatomic, retain) NSNumber *high;
@property (nonatomic, retain) NSNumber *low; 
@property (nonatomic, retain) NSNumber *open;
@property (nonatomic, retain) NSNumber *volume;

@end
