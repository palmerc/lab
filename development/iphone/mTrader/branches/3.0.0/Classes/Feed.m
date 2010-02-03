//
//  Feed.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Feed.h"
#import "Symbol.h"

@implementation Feed
@synthesize feedNumber = _feedNumber;
@synthesize feedDescription = _feedDescription;
@synthesize code = _code;

@synthesize symbols = _symbols;

-(id)init {
	self = [super init];
	if (self != nil) {
		_feedNumber = nil;
		_feedDescription = nil;
		_code = nil;
		_symbols = [[NSMutableArray alloc] init];
	}
	return self;
}

-(void)dealloc {
	[self.feedNumber release];
	[self.feedDescription release];
	[self.code release];
	[self.symbols release];
	[super dealloc];
}

-(NSString *)description {
	return [NSString stringWithFormat:@"(Number: %@, Description: %@, Code: %@)", self.feedNumber, self.feedDescription, self.code];
}

- (BOOL)respondsToSelector:(SEL)aSelector {
	NSLog(@"%", aSelector);
	
	return [super respondsToSelector:aSelector];
}

- (BOOL)isEqual:(id)anObject {
	if ([anObject isMemberOfClass:[Feed class]]) {
		Feed *feed = (Feed *)anObject;
		if ([self.feedNumber isEqual:feed.feedNumber]) {
			return YES;
		}
	}
	return NO;
}

@end
