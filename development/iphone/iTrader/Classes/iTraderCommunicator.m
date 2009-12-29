//
//  iTraderCommunicator.m
//  iTraderCommunicator is a Singleton that the rest of the application can use to
//  communicate with the mTraderServer
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

#import "iTraderCommunicator.h"

@implementation iTraderCommunicator

static iTraderCommunicator *sharedCommunicator = nil;

@synthesize communicator = _communicator;
@synthesize isLoggedIn = _isLoggedIn;

+ (iTraderCommunicator *)sharedManager {
	if (sharedCommunicator == nil) {
		sharedCommunicator = [[super allocWithZone:NULL] init];
	}
	return sharedCommunicator;
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

- (id)init {
	self = [super init];
	if (self != nil) {
		_communicator = [[Communicator alloc] initWithSocket:@"wireless.theonlinetrader.com" port:7780];
		_communicator.delegate = self;
		_isLoggedIn = NO;
		_loginStatusHasChanged = NO;
	}
	return self;
}

- (void)dealloc {
	[self logout];
	[self.communicator stopConnection];
	[self.communicator release];
	[super dealloc];
}

// Delegate method from Communicator
- (void)dataReceived {
	UIApplication* app = [UIApplication sharedApplication]; 
	app.networkActivityIndicatorVisible = YES; 
	NSString *currentLine = [self.communicator readLine];
	NSLog(@"%@", currentLine);
	if ([currentLine rangeOfString:@"Request: login/OK"].location == 0) {
		_loginStatusHasChanged = YES;
		_isLoggedIn = YES;
	} else if ([currentLine rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		_loginStatusHasChanged = YES;
		_isLoggedIn = NO;
	} else if ([currentLine rangeOfString:@"Content-Length:"].location == 0) {
	}
	app.networkActivityIndicatorVisible = NO;
}

- (BOOL)loginStatusHasChanged {
	BOOL result = NO;
	if (_loginStatusHasChanged) {
		result = YES;
		
		_loginStatusHasChanged = NO;
	}
	return result;
}

- (void)logout {
	NSString *ActionLogout = @"Action: Logout";
	NSArray *logoutArray = [NSArray arrayWithObjects:ActionLogout, nil];
	NSString *logoutString = [self arrayToFormattedString:logoutArray];
	[self.communicator writeString:logoutString];
}

- (void)login:(NSString *)username password:(NSString *)password {
	NSString *version = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
	NSString *build = [NSString stringWithFormat:@"%@",[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"]];
	
	NSString *ActionLogin = @"Action: login";
	NSString *Authorization = [[NSString alloc] initWithFormat:@"Authorization: %@/%@", username, password];
	NSString *Platform = [[NSString alloc] initWithFormat:@"Platform: %@ %@", [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion]];
	NSString *Client = @"Client: iTrader";
	NSString *Version = [[NSString alloc] initWithFormat:@"VerType: %@.%@", version, build];
	NSString *ConnectionType = @"ConnType: Socket";
	NSString *Streaming = @"Streaming: 1";
	
	NSArray *loginArray = [NSArray arrayWithObjects:ActionLogin, Authorization, Platform, Client, Version, ConnectionType, Streaming, nil];
	NSString *loginString = [self arrayToFormattedString:loginArray];
	NSLog(@"%@", loginString);
	if ([self.communicator isConnected]) {
		[self.communicator stopConnection];
		_isLoggedIn = NO;
	}
	
	[self.communicator startConnection];
	[self.communicator writeString:loginString];
}

- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings {
	NSString *EOL = @"\r\n";
	
	NSMutableString *appendableText = [[NSMutableString alloc] init];
	for (NSString *string in arrayOfStrings) {
		NSString *current = [[NSString alloc] initWithFormat:@"%@%@", string, EOL];
		[appendableText appendString:current];
		[current release];
	}
	[appendableText appendString:EOL]; // A blank line indicates the end of the sending block
	NSString *immutableString = [NSString stringWithUTF8String:[appendableText UTF8String]];
	[appendableText release];
	
	return immutableString;
}

@end
