//
//  Feed.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Feed.h"


@implementation Feed
@synthesize number, feedDescription, code;

-(id)init {
	self = [super init];
	if (self != nil) {
		number = nil;
		feedDescription = nil;
		code = nil;
	}
	return self;
}

-(NSString *)description {
	return [NSString stringWithFormat:@"(Number: %@, Description: %@, Code: %@)", number, feedDescription, code];
}

-(void)dealloc {
	[number release];
	[feedDescription release];
	[code release];
	[super dealloc];
}

-(BOOL)isEqualToString:(NSString *)aString {
	return YES;
}

-(NSInteger)length {
	return 1;
}

@end
