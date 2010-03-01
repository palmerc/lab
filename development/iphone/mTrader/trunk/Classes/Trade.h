//
//  Trade.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@interface Trade : NSObject {
	NSString *time;
	NSNumber *price;
	NSNumber *volume;
}

@property (nonatomic, retain) NSString *time;
@property (nonatomic, retain) NSNumber *price;
@property (nonatomic, retain) NSNumber *volume;

@end
