//
//  queue.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "queue.h"


@implementation queue
@synthesize theQueue;

- (void)inQueue:(id)anObject {
	[theQueue addObject:anObject];
}

- (id)deQueue {
	[theQueue removeObjectAtIndex:0];
	return 
}

@end
