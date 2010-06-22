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
#import "SymbolDataController.h"

@implementation Starter
@synthesize managedObjectContext = _managedObjectContext;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		[self starter];
	}
	
	return self;
}

// This method's sole job is to kick off Singleton's
- (void)starter {
	[UserDefaults sharedManager];
	[mTraderServerMonitor sharedManager];
	
	SymbolDataController *dataController = [SymbolDataController sharedManager];
	dataController.managedObjectContext = self.managedObjectContext;
}


- (void)dealloc {
	[_managedObjectContext release];
	
	[super dealloc];
}

@end
