//
//  mTraderCommunicator.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

#import "iTraderCommunicator.h"

@implementation iTraderCommunicator
@synthesize communicator = _communicator;
@synthesize isLoggedIn = _isLoggedIn;

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
	[self.communicator stopConnection];
	[self.communicator release];
	[super dealloc];
}

// Delegate method from Communicator
- (void)dataReceived {
	NSString *currentLine = [self.communicator readLine];
	NSLog(@"%@", currentLine);
	if ([currentLine rangeOfString:@"Request: login/OK"].location == 0) {
		_loginStatusHasChanged = YES;
		self.isLoggedIn = YES;
	} else if ([currentLine rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		_loginStatusHasChanged = YES;
		self.isLoggedIn = NO;
	} else if ([currentLine rangeOfString:@"Content-Length:"].location == 0) {
	}
}

- (BOOL)loginStatusHasChanged {
	BOOL result = NO;
	if (_loginStatusHasChanged) {
		result = YES;
		
		_loginStatusHasChanged = NO;
	}
	return result;
}

- (void)login:(NSString *)username password:(NSString *)password {
	if ([self.communicator isConnected]) {
		[self.communicator stopConnection];
		self.isLoggedIn = NO;
	}
	
	[self.communicator startConnection];
	NSString *loginString = [[NSString alloc] initWithFormat:@"\r\n\r\nAction: login\r\nAuthorization: %@/%@\r\n", username, password];
	[self.communicator writeString:loginString];
	[loginString release];
}

@end
