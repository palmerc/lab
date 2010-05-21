//
//  mTraderServerMonitor.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

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
		_isLoggedIn = NO;
		
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
	BOOL connectionRequired = [curReach connectionRequired];
	
	NSString *status = nil;
	switch (netStatus) {
		case NotReachable:
			status = @"Not reachable";
			break;
		case ReachableViaWiFi:
			status = @"Reachable via WiFi";
			[self attemptConnection];
			break;
		case ReachableViaWWAN:
			status = @"Reachable via WWAN";
			[self attemptConnection];
			break;
		default:
			break;
	}
	
	NSLog(@"Network Status: %@  Required: %d", status, connectionRequired);
}

- (void)reachabilityChanged:(NSNotification *)note {
	Reachability *reach = [note object];
	NSParameterAssert([reach isKindOfClass:[Reachability class]]);
	
	[self updateReachability:reach];
}

- (void)startReachability {
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
	
	NSMutableCharacterSet *ipAddrSet = [[[NSMutableCharacterSet alloc] init] autorelease];
	[ipAddrSet addCharactersInString:@"0123456789."];
	
	NSArray *characters = [self.server componentsSeparatedByCharactersInSet:ipAddrSet];
	if ([characters count] - 1 == [self.server length]) {
		struct sockaddr_in hostAddress;
		bzero(&hostAddress, sizeof(hostAddress));
		hostAddress.sin_len = sizeof(hostAddress);
		hostAddress.sin_family = AF_INET;
		const char* addr = [self.server cStringUsingEncoding:NSASCIIStringEncoding];
		hostAddress.sin_addr.s_addr = inet_addr(addr);
		hostAddress.sin_port = [self.port integerValue];
		_reachability = [[Reachability reachabilityWithAddress:&hostAddress] retain];
	} else {
		_reachability = [[Reachability reachabilityWithHostName:self.server] retain];
	}
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
	if (_isConnected == NO) {
		[[mTraderCommunicator sharedManager].communicator startConnection];
	}
	
	if (_isConnected && !_isLoggedIn && [self hasUsernameAndPasswordDefined]) {
		[[mTraderCommunicator sharedManager] login];
	}
}


#pragma mark -
#pragma mark mTraderCommunicatorMonitorDelegate methods

- (void)connected {
	_isConnected = YES;
	if (_isLoggedIn == NO) {
		[self attemptConnection];
	}
}

- (void)disconnected {
	if (_isConnected == NO && _isLoggedIn == NO) {
		return;
	}
	_isConnected = NO;
	_isLoggedIn = NO;
	
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Disconnected" message:@"Your phone is unable to reach The Online Trader server. We will automatically connect when it becomes available." delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
	[alertView show];
	[alertView release];	
}

- (void)loginFailed:(NSString *)message {
	_isLoggedIn = NO;
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Login Failed" message:@"Your username or password are incorrect or you lack sufficient rights to access mTrader." delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
	[alertView show];
	[alertView release];
}

- (void)loginSuccessful {
	_isLoggedIn = YES;
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
