//
//  Ask.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@interface Ask : NSObject {
	NSNumber *askValue;
	NSNumber *askSize;
	CGFloat percent;
}

@property (nonatomic, retain) NSNumber *askValue;
@property (nonatomic, retain) NSNumber *askSize;
@property (assign) CGFloat percent;

@end
