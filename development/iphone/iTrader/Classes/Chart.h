//
//  Chart.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 13.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Chart : NSObject {
	NSString *_imageType;
	NSString *_feedTicker;
	NSUInteger _size;
	NSUInteger _height;
	NSUInteger _width;
	UIImage *_image;
}

@property (nonatomic, retain) NSString *imageType;
@property (nonatomic, retain) NSString *feedTicker;
@property NSUInteger size;
@property NSUInteger height;
@property NSUInteger width;
@property (nonatomic, retain) UIImage *image;

@end