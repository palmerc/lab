//
//  Chart.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 13.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Chart.h"


@implementation Chart
@synthesize imageType = _imageType;
@synthesize feedTicker =_feedTicker;
@synthesize size = _size;
@synthesize height = _height;
@synthesize width = _width;
@synthesize image = _image;

- (NSString *)description {
	return [NSString stringWithFormat:@"Chart for feedTicker %@ of type %@ and %d bytes. %d pixels high by %d pixels wide", self.feedTicker, self.imageType, self.size, self.height, self.width];
}

@end
