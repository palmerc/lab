//
//  Feed.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Feed : NSObject {
	NSNumber *number;
	NSString *description;
	NSString *code;
}

@property (nonatomic,retain) NSNumber *number;
@property (nonatomic,retain) NSString *description;
@property (nonatomic,retain) NSString *code;

@end