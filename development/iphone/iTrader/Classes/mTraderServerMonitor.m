//
//  mTraderServerMonitor.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderServerMonitor.h"
#import "iTraderCommunicator.h"
#import "Reachability.h"


@implementation mTraderServerMonitor

static mTraderServerMonitor *sharedMonitor = nil;
@synthesize reachability = _reachability;

#pragma mark Initialization and Cleanup
-(id) init {
	self = [super init];
	if (self != nil) {
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
		self.reachability = [Reachability reachabilityWithHostName:@"wireless.theonlinetrader.com"];
		[self.reachability startNotifer];
	}
	return self;
}

-(void) dealloc {
	[self.reachability stopNotifer];
	[super dealloc];
}

#pragma mark Singleton Methods
/**
 * Methods for Singleton implementation
 *
 */
+ (mTraderServerMonitor *)sharedManager {
	if (sharedMonitor == nil) {
		sharedMonitor = [[super allocWithZone:NULL] init];
	}
	return sharedMonitor;
}

+ (id)allocWithZone:(NSZone *)zone {
	return [[self sharedManager] retain];
}

- (id)copyWithZone:(NSZone *)zone {
	return self;
}

- (id)retain {
	return self;
}

- (NSUInteger)retainCount {
	return NSUIntegerMax;
}

- (void)release {
	// do nothing
}

- (id)autorelease {
	return self;
}

#pragma mark Reachability
/**
 * Delegate methods from Communicator
 *
 */

- (void)reachabilityChanged:(NSNotification *)note {
	Reachability *reachNoteObject= [note object];
	NetworkStatus status = [reachNoteObject currentReachabilityStatus];
	
	if (status == NotReachable) {
		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Disconnected" message:@"Your phone is unable to reach The Online Trader server. We will automatically connect when it becomes available." delegate:self cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
		[alertView show];
		[alertView release];
	} else {
		[[iTraderCommunicator sharedManager] login];
	}
	
	NSLog(@"Reachability is %d", status);
}

@end
