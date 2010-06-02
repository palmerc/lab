//
//  Starter.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


@interface Starter : NSObject {
}

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)starter:(NSManagedObjectContext *)managedObjectContext;

@end
