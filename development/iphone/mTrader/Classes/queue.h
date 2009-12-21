//
//  queue.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface queue : NSObject {
	NSMutableArray *theQueue;
}

@property (nonatomic, retain) NSMutableArray *theQueue;

- (void)inQueue:(id)anObject;
- (id)deQueue;

@end
