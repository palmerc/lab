//
//  mTraderServerMonitor.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#define DEBUG_COMMUNICATOR_STATUS 1
#define DEBUG_REACHABILITY 1

#import "Monitor.h"

#import "mTraderCommunicator.h"
#import "UserDefaults.h"
#import "CPHost.h"

#import "StatusController.h"

#include <netdb.h>
#import <arpa/inet.h>


@interface Monitor ()
- (BOOL)hasUsernameAndPasswordDefined;
@end


@implementation Monitor

static Monitor *sharedMonitor = nil;
@synthesize host = _host;
@synthesize port = _port;
@synthesize statusController = _statusController;
@synthesize connected = _connected;
@synthesize loggedIn = _loggedIn;

#pragma mark -
#pragma mark Singleton Methods
/**
 * Methods for Singleton implementation
 *
 */
+ (Monitor *)sharedManager {
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
#pragma mark Initialization
- (id)init {
	self = [super init];
	if (self != nil) {
		_reachability = nil;

		_loggedIn = NO;
		_connected = NO;
		_connecting = NO;
		_statusController = nil;
		
		_host = [[NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerAddress"]] retain];
		NSString *urlString = [NSString stringWithFormat:@"socket://%@", _host];
		_url = [[NSURL URLWithString:urlString] retain];
		NSString *port = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerPort"]];
		_port = [port integerValue]; 
	}
	return self;
}

#pragma mark -
#pragma mark Reachability

- (NetworkStatus)currentReachabilityStatus {
	return [_reachability currentReachabilityStatus];
}

- (void)updateReachability:(Reachability *)curReach {
	NetworkStatus netStatus = [curReach currentReachabilityStatus];
	
#if DEBUG_REACHABILITY
	NSString *status = nil;
#endif
	switch (netStatus) {
		case NotReachable:
#if DEBUG_REACHABILITY
			status = @"Not reachable";
#endif
			break;
		case ReachableViaWiFi:
#if DEBUG_REACHABILITY
			status = @"Reachable via WiFi";
#endif
			[self applicationDidBecomeActive];
			break;
		case ReachableViaWWAN:
#if DEBUG_REACHABILITY
			[self applicationDidBecomeActive];
			status = @"Reachable via WWAN";
#endif
			break;
		default:
			break;
	}
#if DEBUG_REACHABILITY
	BOOL connectionRequired= [curReach connectionRequired];
	
	NSLog(@"Network Status: %@  Required: %d", status, connectionRequired);
#endif
}

- (void)reachabilityChanged:(NSNotification *)note {
	Reachability *reach = [note object];
	NSParameterAssert([reach isKindOfClass:[Reachability class]]);
	
	[self updateReachability:reach];
}

#pragma mark -
#pragma mark Application State Changes
// Application started
- (void)applicationDidFinishLaunching {
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
	
	_reachability = [[Reachability reachabilityWithHostName:[_url host]] retain];
	
	[_reachability startNotifer];
	
	// Connect the pieces of the communication stack
	_mTCom = [mTraderCommunicator sharedManager];
	_mTCom.statusDelegate = self;
	
	mTraderLinesToBlocks *linesToBlocks = [[mTraderLinesToBlocks alloc] init];
	linesToBlocks.dataDelegate = _mTCom;
	
	LineOrientedCommunication *lineOrientedCommunication = [[LineOrientedCommunication alloc] init];
	lineOrientedCommunication.dataDelegate = linesToBlocks;	
	
	_communicator = [[Communicator alloc] init];
	_mTCom.communicator = _communicator;
	_communicator.statusDelegate = self;
	_communicator.dataDelegate = lineOrientedCommunication;
}

// Application quitting
- (void)applicationWillTerminate {
	[_mTCom logout];
	
	[_communicator stopConnection];
}

// Phone woke up
- (void)applicationDidBecomeActive {
	if (_connecting == NO && [self hasUsernameAndPasswordDefined]) {
		_connecting = YES;
		
		NSString *message = NSLocalizedString(@"connecting", @"Connecting");
		self.statusController.statusMessage = message;
		[self.statusController displayStatus];
	
		[_communicator startConnectionWithSocket:_url onPort:_port];
	}
}

// Phone went to sleep
- (void)applicationWillResignActive {
	[_mTCom logout];

	[_communicator stopConnection];
}

- (void)usernameAndPasswordChanged {
	//
}

#pragma mark -
#pragma mark CommunicatorStatusDelegate methods

- (void)connect {
#if DEBUG_COMMUNICATOR_STATUS
	NSLog(@"Monitor: connect");
#endif
	if (_connected == NO) {
		_connecting = NO;
		_connected = YES;
		NSString *message = NSLocalizedString(@"connected", @"Connected");
		self.statusController.statusMessage = message;
		[self.statusController displayStatus];
		
		if (!self.loggedIn) {
			[[mTraderCommunicator sharedManager] login];
		}
	}
}

- (void)disconnect {	
#if DEBUG_COMMUNICATOR_STATUS
	NSLog(@"Monitor: disconnect");
#endif
	
	NSString *message = NSLocalizedString(@"disconnected", @"Disconnected");
	self.statusController.statusMessage = message;
	[self.statusController displayStatus];
	
	_connected = NO;
	_loggedIn = NO;
}

#pragma mark -
#pragma mark mTraderStatusDelegate methods

- (void)loginSuccessful {
	NSString *message = NSLocalizedString(@"loggedIn", @"Logged In");
	self.statusController.statusMessage = message;
	[self.statusController hideStatus];
	
	_loggedIn = YES;
}

- (void)loginFailed:(NSString *)message {
	_loggedIn = NO;
		
	//_statusAlertView = [[UIAlertView alloc] initWithTitle:@"Login Failed" message:@"Your username or password are incorrect or you lack sufficient rights to access mTrader." delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
	//[_statusAlertView show];
}

- (void)kickedOut {
	[_reachability stopNotifer];
	
#if DEBUG_COMMUNICATOR_STATUS
	NSLog(@"Monitor: Kicked out");
#endif
	[[mTraderCommunicator sharedManager].communicator stopConnection];
	
	
	//_statusAlertView = [[UIAlertView alloc] initWithTitle:@"Kickout" message:@"You have been logged off since you logged in from another client. This application will terminate." delegate:self cancelButtonTitle:@"Quit" otherButtonTitles:nil];
	//[_statusAlertView show];
}

#pragma mark -
#pragma mark Application Exit After Kickout

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex {
	exit(0);
}

#pragma mark -
#pragma mark Bytes Sent and Received
- (NSUInteger)bytesSent {
	return _communicator.bytesSent;
}

- (NSUInteger)bytesReceived {
	return _communicator.bytesReceived;
}

#pragma mark -
#pragma mark Helper methods
- (BOOL)hasUsernameAndPasswordDefined {
	UserDefaults *userDefaults = [UserDefaults sharedManager];
	NSString *username = userDefaults.username;
	NSString *password = userDefaults.password;
	
	if (username == nil || password == nil || [username isEqualToString:@""] || [password isEqualToString:@""]) {
			NSString *title = @"Username and/or password missing";
			NSString *message = @"Please add your username and password for the mTrader service in the setting's tab.";
			NSString *cancelButtonTitle = @"Dismiss";
			
			UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:nil cancelButtonTitle:cancelButtonTitle otherButtonTitles:nil];
			[alert show];
			[alert release];
		return NO;
	}
	
	return YES;
}

#pragma mark -
#pragma mark Network Diagnostic methods

- (NSArray *)serverAddresses {
	const char *hostname = [_host cStringUsingEncoding:NSASCIIStringEncoding];
	struct hostent *remoteHostEnt = gethostbyname(hostname);
	char **list;
	
	NSMutableArray *addresses = [NSMutableArray array];
	if (remoteHostEnt != NULL) {
		list = remoteHostEnt->h_addr_list;	
		
		for (int i = 0; i < sizeof(list) / sizeof(struct in_addr *); i++) {
			struct in_addr *ip = (struct in_addr *)list[i];
			inet_ntoa(*ip);
			NSString *ipAddress = [NSString stringWithCString:inet_ntoa(*ip) encoding:NSASCIIStringEncoding];
			[addresses addObject:ipAddress];
		}
	}
	
	return addresses;
}

- (NSArray *)interfaces {
	NSMutableArray *interfaces = [NSMutableArray array];
	NSDictionary *interfacesToAddresses = [CPHost interfacesToAddresses];
	for (NSString *key in [interfacesToAddresses allKeys]) {
		[interfaces addObject:[NSString stringWithFormat:@"%@: %@", key, [interfacesToAddresses objectForKey:key]]];
	}
	
	return (NSArray *)interfaces;
}

#pragma mark -
#pragma mark Memory management
-(void) dealloc {
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	[_statusController release];
	[_host release];
	[_url release];
	[_reachability stopNotifer];
	[_reachability release];
	[super dealloc];
}

@end
