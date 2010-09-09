//
//  mTraderServerMonitor.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#define DEBUG_COMMUNICATOR_STATUS 0
#define DEBUG_REACHABILITY 0
#define DEBUG_LIFECYCLE 0
#define DEBUG_STATE 0

#import "Monitor.h"

#import "mTraderCommunicator.h"
#import "UserDefaults.h"
#import "CPHost.h"

#import "StatusController.h"

#include <netdb.h>
#import <arpa/inet.h>


@interface Monitor ()
- (BOOL)hasUsernameAndPasswordDefined;
- (void)setup;
- (void)start;
- (void)login;
- (void)logout;
@end


@implementation Monitor

static Monitor *sharedMonitor = nil;
@synthesize mTraderHost = _mTraderHost;
@synthesize mTraderPort = _mTraderPort;
@synthesize mTradingHost = _mTradingHost;
@synthesize mTradingPort = _mTradingPort;
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
		_communicator = nil;
		_mTradingCommunicator = nil;
		_mTCom = nil;
		
		_reachability = nil;

		_loggedIn = NO;
		_connected = NO;
		_connecting = NO;
		_statusController = nil;
		
		_mTraderHost = [[NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerAddress"]] retain];
		NSString *mTraderURLString = [NSString stringWithFormat:@"socket://%@", _mTraderHost];
		_mTraderURL = [[NSURL URLWithString:mTraderURLString] retain];
		NSString *mTraderPortString = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerPort"]];
		_mTraderPort = [mTraderPortString integerValue];
		
		_mTradingHost = [[NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTradingServerAddress"]] retain];
		NSString *mTradingURLString = [NSString stringWithFormat:@"socket://%@", _mTradingHost];
		_mTradingURL = [[NSURL URLWithString:mTradingURLString] retain];
		NSString *mTradingPortString = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTradingServerPort"]];
		_mTradingPort = [mTradingPortString integerValue];
	}
	return self;
}

- (void)setup {
	if (_state > SETUP) {
		return;
	}
#if DEBUG_STATE
	NSLog(@"Monitor: SETUP");
#endif
	_state = SETUP;
	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
	_reachability = [[Reachability reachabilityWithHostName:[_mTraderURL host]] retain];
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
	
	//_mTradingCommunicator = [[Communicator alloc] init];
	//_mTradingCommunicator.tlsEnabled = NO;
	
}

- (void)start {
	if (_state >= START) {
		return;
	}
#if DEBUG_STATE
	NSLog(@"Monitor: START");
#endif
	_state = START;

	if (![self hasUsernameAndPasswordDefined]) {
		if (_state == NOUSERNAMEORPASSWORD) {
			return;
		}
#if DEBUG_STATE
		NSLog(@"Monitor: NOUSERNAMEORPASSWORD");
#endif
		_state = NOUSERNAMEORPASSWORD;
		
		NSString *title = @"Username and/or password missing";
		NSString *message = @"Please add your username and password for the mTrader service in the setting's tab.";
		NSString *cancelButtonTitle = @"Dismiss";
		
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:nil cancelButtonTitle:cancelButtonTitle otherButtonTitles:nil];
		[alert show];
		[alert release];
		
		// permanent disconnect
	} else {
		if (_state == CONNECTING) {
			return;
		}
#if DEBUG_STATE
		NSLog(@"Monitor: CONNECTING");
#endif

		_state = CONNECTING;
		NSString *message = NSLocalizedString(@"connecting", @"Connecting");
		self.statusController.statusMessage = message;
		[self.statusController displayStatus];
		
		[_communicator startConnectionWithSocket:_mTraderURL onPort:_mTraderPort];
		//[_mTradingCommunicator startConnectionWithSocket:_mTradingURL onPort:_mTradingPort];
		
		// This method expects that after a certain short interval to receive a call to connect:
	}
}

- (void)login {
	if (_state == LOGGINGIN) {
		return;
	}
#if DEBUG_STATE
	NSLog(@"Monitor: LOGGINGIN");
#endif
	_state = LOGGINGIN;
	NSString *message = NSLocalizedString(@"loggingIn", @"Logging In");
	self.statusController.statusMessage = message;
	[self.statusController displayStatus];
	[[mTraderCommunicator sharedManager] login];

	//This method expects a call to loginSuccessful: or loginFailed:
}
- (void)logout {
	if (_state == LOGOUT || _state == DISCONNECTED) {
		return;
	}
#if DEBUG_STATE
	NSLog(@"Monitor: LOGOUT");
#endif
	
	_state = LOGOUT;
	
	[_mTCom logout];
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
			[self start];
			break;
		case ReachableViaWWAN:
#if DEBUG_REACHABILITY
			status = @"Reachable via WWAN";
#endif
			[self start];
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
#if DEBUG_LIFECYCLE
	NSLog(@"Monitor: applicationDidFinishLaunching");
#endif
	[self setup];
}

// Application quitting
- (void)applicationWillTerminate {
#if DEBUG_LIFECYCLE
	NSLog(@"Monitor: applicationWillTerminate");
#endif
	[self logout];
}

// Phone woke up
- (void)applicationDidBecomeActive {
#if DEBUG_LIFECYCLE
	NSLog(@"Monitor: applicationDidBecomeActive");
#endif
	[self start];
}

// Phone went to sleep
- (void)applicationWillResignActive {
#if DEBUG_LIFECYCLE
	NSLog(@"Monitor: applicationWillResignActive");
#endif
	[_mTCom logout];

	[_communicator stopConnection];
	
	_state = DISCONNECTED;
	_loggedIn = NO;
	_connected = NO;
	_connecting = NO;
}

- (void)usernameAndPasswordChanged {	
	if ([self hasUsernameAndPasswordDefined]) {
		_state = DISCONNECTED;
		[self start];
	} else {
		[self logout];
	}
}

#pragma mark -
#pragma mark CommunicatorStatusDelegate methods

- (void)connect {
#if DEBUG_COMMUNICATOR_STATUS
	NSLog(@"Monitor: connect");
#endif
	_state = CONNECTED;
	NSString *message = NSLocalizedString(@"connected", @"Connected");
	self.statusController.statusMessage = message;
	[self.statusController displayStatus];
		
	[self login];
}

- (void)disconnect {	
#if DEBUG_COMMUNICATOR_STATUS
	NSLog(@"Monitor: disconnect");
#endif
	_state = DISCONNECTED;
	[_communicator stopConnection];
	NSString *message = NSLocalizedString(@"disconnected", @"Disconnected");
	self.statusController.statusMessage = message;
	[self.statusController displayStatus];
}

#pragma mark -
#pragma mark mTraderStatusDelegate methods

- (void)loginSuccessful {
	_connecting = NO;
	_connected = NO;
	_loggedIn = YES;

	NSString *message = NSLocalizedString(@"loggedIn", @"Logged In");
	self.statusController.statusMessage = message;
	[self.statusController hideStatus];
}

- (void)loginFailed:(NSString *)message {
	[_reachability stopNotifer];
	[[mTraderCommunicator sharedManager].communicator stopConnection];
	
	_connecting = NO;
	_connected = NO;
	_loggedIn = NO;
	
	NSString *title = @"Login Failed";
	NSString *messageString = @"Your username or password are incorrect or you lack sufficient rights to access mTrader.";
	NSString *cancelButton = @"Dismiss";
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:messageString delegate:nil cancelButtonTitle:cancelButton otherButtonTitles:nil];
	[alert show];
	[alert release];
}

- (void)kickedOut {
#if DEBUG_COMMUNICATOR_STATUS
	NSLog(@"Monitor: Kicked out");
#endif
	[_reachability stopNotifer];
	[[mTraderCommunicator sharedManager].communicator stopConnection];
	_connecting = NO;
	_connected = NO;
	_loggedIn = NO;
	
	NSString *title = @"Kickout";
	NSString *message = @"You have been logged off since you logged in from another client. This application will terminate.";
	NSString *cancelButton = @"Quit";
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:cancelButton otherButtonTitles:nil];
	[alert show];
	[alert release];
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
		return NO;
	}
	
	return YES;
}

#pragma mark -
#pragma mark Network Diagnostic methods

- (NSArray *)serverAddresses {
	const char *hostname = [_mTraderHost cStringUsingEncoding:NSASCIIStringEncoding];
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
	[_mTraderHost release];
	[_mTraderURL release];
	[_mTradingHost release];
	[_mTradingURL release];
	[_reachability stopNotifer];
	[_reachability release];
	[_communicator release];
	[_mTradingCommunicator release];
	[_mTCom release];
	
	[super dealloc];
}

@end

