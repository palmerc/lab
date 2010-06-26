//
//  mTraderServerMonitor.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#define DEBUG 0

#import "mTraderServerMonitor.h"

#import "mTraderCommunicator.h"
#import "UserDefaults.h"

#import "Reachability.h"

#import <arpa/inet.h>

@implementation mTraderServerMonitor

static mTraderServerMonitor *sharedMonitor = nil;
@synthesize reachability = _reachability;
@synthesize server = _server;
@synthesize port = _port;

#pragma mark -
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

#pragma mark -
#pragma mark Initialization and Cleanup
- (id)init {
	self = [super init];
	if (self != nil) {
		isLoggedIn = NO;
		
		self.server = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerAddress"]];
		self.port = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerPort"]];
		_reachability = nil;
		
				
		[mTraderCommunicator sharedManager].mTraderServerMonitorDelegate = self;
		
		[self startReachability];
	}
	return self;
}

#pragma mark -
#pragma mark Reachability

- (void)updateReachability:(Reachability *)curReach {
	NetworkStatus netStatus = [curReach currentReachabilityStatus];
	
#if DEBUG
	NSString *status = nil;
#endif
	switch (netStatus) {
		case NotReachable:
#if DEBUG
			status = @"Not reachable";
#endif
			break;
		case ReachableViaWiFi:
#if DEBUG
			status = @"Reachable via WiFi";
#endif
			[self attemptConnection];
			break;
		case ReachableViaWWAN:
#if DEBUG
			status = @"Reachable via WWAN";
#endif
			[self attemptConnection];
			break;
		default:
			break;
	}
#if DEBUG
	BOOL connectionRequired= [curReach connectionRequired];

	NSLog(@"Network Status: %@  Required: %d", status, connectionRequired);
#endif
}

- (void)reachabilityChanged:(NSNotification *)note {
	Reachability *reach = [note object];
	NSParameterAssert([reach isKindOfClass:[Reachability class]]);
	
	[self updateReachability:reach];
}

- (void)startReachability {
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
	
	_reachability = [[Reachability reachabilityWithHostName:self.server] retain];

	[self.reachability startNotifer];	
}

- (BOOL)hasUsernameAndPasswordDefined {
	UserDefaults *userDefaults = [UserDefaults sharedManager];
	NSString *username = userDefaults.username;
	NSString *password = userDefaults.password;
	
	if (username == nil || password == nil || [username isEqualToString:@""] || [password isEqualToString:@""]) {
		return NO;
	}
	
	return YES;
}

- (void)attemptConnection {
	if (isConnected == NO) {
		[[mTraderCommunicator sharedManager].communicator startConnection];
	}
	
	if (isConnected && !isLoggedIn && [self hasUsernameAndPasswordDefined]) {
		[[mTraderCommunicator sharedManager] login];
	}
}

- (void)disconnect {
	if (isConnected == NO) {
		return;
	}
	
	[[mTraderCommunicator sharedManager] logout];
	[[mTraderCommunicator sharedManager].communicator stopConnection];
	isConnected = NO;
	isLoggedIn = NO;
}

#pragma mark -
#pragma mark mTraderCommunicatorMonitorDelegate methods

- (void)connected {
	isConnected = YES;
	if (isLoggedIn == NO) {
		[self attemptConnection];
	}
}

- (void)disconnected {
	if (isConnected == NO && isLoggedIn == NO) {
		return;
	}
	isConnected = NO;
	isLoggedIn = NO;
	
	[[mTraderCommunicator sharedManager].communicator stopConnection];
}

- (void)loginFailed:(NSString *)message {
	isLoggedIn = NO;
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Login Failed" message:@"Your username or password are incorrect or you lack sufficient rights to access mTrader." delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
	[alertView show];
	[alertView release];
}

- (void)loginSuccessful {
	isLoggedIn = YES;
}

-(void) kickedOut {
	[self.reachability stopNotifer];
	NSLog(@"Kicked out");
	[[mTraderCommunicator sharedManager].communicator stopConnection];
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Kickout" message:@"You have been logged off since you logged in from another client. This application will terminate." delegate:self cancelButtonTitle:@"Quit" otherButtonTitles:nil];
	[alertView show];
	[alertView release];
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex {
	exit(0);
}

#pragma mark -
#pragma mark Memory management
-(void) dealloc {
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	[_server release];
	[_port release];
	[_reachability stopNotifer];
	[_reachability release];
	[super dealloc];
}

@end
