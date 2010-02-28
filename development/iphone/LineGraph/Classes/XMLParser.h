//
//  XMLParser.h
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 28.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


@interface XMLParser : NSObject {
@private
	NSMutableArray *points;
}

@property (nonatomic, readonly) NSMutableArray *points;

@end
