//
//  Bid.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@interface Bid : NSObject {
	NSNumber *bidValue;
	NSNumber *bidSize;
	CGFloat percent;	
}

@property (nonatomic, retain) NSNumber *bidValue;
@property (nonatomic, retain) NSNumber *bidSize;
@property (assign) CGFloat percent;

@end
