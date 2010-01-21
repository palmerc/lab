//
//  Starter.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Starter.h"
#import "UserDefaults.h"
#import "SymbolsController.h"
#import "iTraderCommunicator.h"
#import "mTraderServerMonitor.h"

@implementation Starter

- (id)init {
	self = [super init];
	if (self != nil) {
		[self starter];
	}
	
	return self;
}

// This method's sole job is to kick off Singleton's
- (void)starter {
	[UserDefaults sharedManager];
	[SymbolsController sharedManager];
	[mTraderServerMonitor sharedManager];
	
	// Start the networking bit last.
	[iTraderCommunicator sharedManager];
	//NSLog(@"Startup of Singletons complete.");
}


- (void)dealloc {
	// We didn't allocate anything
	[super dealloc];
}

@end
