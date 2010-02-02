//
//  queue.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

@interface NSMutableArray (QueueAdditions) 

-(void) enQueue:(id)anObject;
-(id) deQueue;

@end
