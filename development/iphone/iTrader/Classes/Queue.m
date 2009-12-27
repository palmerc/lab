//
//  queue.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "Queue.h"


@implementation NSMutableArray (QueueAdditions)

- (void)enQueue:(id)anObject {
	[self addObject:anObject];
}

- (id)deQueue {
	id headObject = [self objectAtIndex:0];
	if (headObject != nil) {
		[[headObject retain] autorelease];
		[self removeObjectAtIndex:0];
	}
	return headObject;
}

@end
