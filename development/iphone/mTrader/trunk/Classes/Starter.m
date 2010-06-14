//
//  Starter.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Starter.h"
#import "UserDefaults.h"
#import "mTraderServerMonitor.h"
#import "DataController.h"

@implementation Starter

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		[self starter:managedObjectContext];
	}
	
	return self;
}

// This method's sole job is to kick off Singleton's
- (void)starter:(NSManagedObjectContext *)managedObjectContext {
	[UserDefaults sharedManager];
	[mTraderServerMonitor sharedManager];
	
	DataController *dataController = [DataController sharedManager];
	dataController.managedObjectContext = managedObjectContext;
}


- (void)dealloc {
	[super dealloc];
}

@end
