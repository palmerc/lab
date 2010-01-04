//
//  Feed.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Feed.h"


@implementation Feed
@synthesize number, description, code;

-(NSString *)description {
	return [NSString stringWithFormat:@"(Number: %@, Description: %@, Code: %@)", number, description, code];
}

@end
