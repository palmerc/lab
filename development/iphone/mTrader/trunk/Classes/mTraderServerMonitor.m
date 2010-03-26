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
		isLoggedIn = NO;
		
		self.server = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerAddress"]];
		self.port = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerPort"]];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
		
		
		[mTraderCommunicator sharedManager].mTraderServerMonitorDelegate = self;
		
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
			self.reachability = [Reachability reachabilityWithAddress:&hostAddress];
		} else {
			self.reachability = [Reachability reachabilityWithHostName:self.server];
		}
		
		[self.reachability startNotifer];
	}
	return self;
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
	if (isConnected && !isLoggedIn && hasUsernameAndPasswordDefined) {
		[[mTraderCommunicator sharedManager] login];
	}
}

#pragma mark -
#pragma mark Reachability
/**
 * Delegate methods from Communicator
 *
 */

- (void)reachabilityChanged:(NSNotification *)note {
	Reachability *reachNoteObject = [note object];
	NetworkStatus status = [reachNoteObject currentReachabilityStatus];
	
	if (status == NotReachable) {
		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Disconnected" message:@"Your phone is unable to reach The Online Trader server. We will automatically connect when it becomes available." delegate:self cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
		[alertView show];
		[alertView release];
	} else {
		//[self attemptConnection];
	}
	
	NSLog(@"Reachability is %d", status);
}

#pragma mark -
#pragma mark CommunicatorMonitorDelegate methods

- (void)connected {
	isConnected = YES;
	[self attemptConnection];
}

- (void)disconnected {
	isConnected = NO;
	isLoggedIn = NO;
}

#pragma mark -
#pragma mark mTraderCommunicatorMonitorDelegate methods

- (void)loginFailed:(NSString *)message {
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Login Failed" message:@"Your username or password are incorrect or you lack sufficient rights to access mTrader." delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
	[alertView show];
	[alertView release];
}

- (void)loginSuccessful {
	isLoggedIn = YES;
}


-(void) kickedOut {
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
	[self.server release];
	[self.port release];
	[self.reachability stopNotifer];
	[self.reachability release];
	[super dealloc];
}

@end
