//
//  queue.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface NSMutableArray (QueueAdditions) 

- (void)enQueue:(id)anObject;
- (id)deQueue;

@end
