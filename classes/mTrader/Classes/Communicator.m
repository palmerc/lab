//
//  Communicator.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "Communicator.h"

@implementation Communicator

@synthesize username, password, inputStream, outputStream, isConnected;

- (id)init {
	self = [super init];
	
	if (self == nil) {
		self.username = nil;
		self.password = nil;
	}
	return self;
}

- (id)initWithUsernameAndPassword:(NSString *)_username password:(NSString *)_password {
	self = [super init];
	if (self == nil) {
		self.username = _username;
		self.password = _password;
	}
	
	return self;
}

- (void)setup {
	NSString *urlString = @"socket://wireless.theonlinetrader.com";
	NSURL *url = [NSURL URLWithString:urlString];
	
	NSLog(@"%@", url);
	CFReadStreamRef readStream;
	CFWriteStreamRef writeStream;
	CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)[url host], 7780, &readStream, &writeStream);
		
	inputStream = (NSInputStream *)readStream;
	outputStream = (NSOutputStream *)writeStream;
	
	self.inputStream.delegate = self;
	self.outputStream.delegate = self;
	
	[self.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[self.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[self.inputStream open];
	[self.outputStream open];
}

- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode {
	NSString *eventString;
	switch(eventCode) {
		case NSStreamEventNone:
			eventString = @"NSStreamEventNone";
			break;
		case NSStreamEventOpenCompleted:
			eventString = @"NSStreamEventOpenCompleted";
			break;
		case NSStreamEventHasBytesAvailable:
			eventString = @"NSStreamEventHasBytesAvailable";
			if (stream == inputStream) {
				NSInteger bytesRead;
				buffer[32768];
				
				bytesRead = [inputStream read:buffer maxLength:sizeof(buffer)];
				if (bytesRead == -1) {
					NSLog(@"Network read error");
				} else if (bytesRead == 0) {
					NSLog(@"stopReceiveWithStatus:nil");
					//[self _stopReceiveWithStatus:nil];
				} else {
					NSLog(@"Buffer contents: %s", buffer);
					
				}
				[inputStream close];
			}
			break;
		case NSStreamEventHasSpaceAvailable:
			eventString = @"NSStreamEventHasSpaceAvailable";
			if (stream == outputStream) {
				self.username = @"cameron";
				self.password = @"Ct1gg3rR";
				
				NSString *loginString = [NSString stringWithFormat:@"\r\n\r\nAction: login\r\nAuthorization: %@/%@\r\n", self.username, self.password];
				const uint8_t *rawLoginString = (const uint8_t *)[loginString UTF8String];
				[outputStream write:rawLoginString maxLength:strlen(rawLoginString)];
				[outputStream close];
			}
			break;
		case NSStreamEventErrorOccurred:
			eventString = @"NSStreamEventErrorOccurred";
			break;
		case NSStreamEventEndEncountered:
			eventString = @"NSStreamEventEndEncountered";
			break;
		default:
			NSLog(@"Unhandled eventcode: %d", eventCode);
	}
	NSLog(@"eventCode: %@", eventString);
}

- (BOOL)login {	
	[self setup];
	NSLog(@"Buffer contents: %s", buffer);
	return YES;
}

@end
