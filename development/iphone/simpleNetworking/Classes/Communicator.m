//
//  Communicator.m
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Bird And Bear Productions. All rights reserved.
//

#import "Communicator.h"


@implementation Communicator

@synthesize host, port, inputStream, outputStream, isConnected;

- (id)init {
	self = [super init];
	if (self == nil) {
		host = nil;
		port = 0;
		inputStream = nil;
		outputStream = nil;
	}
	return self;
}

- (id)initWithSocket:(NSString *)_host port:(NSInteger)_port {
	self = [super init];
	if (self == nil) {
		self.host = _host;
		self.port = _port;
		
	}
	return self;
}



- (void)start {
	CFWriteStreamRef writeStream;
	CFReadStreamRef readStream;
	
	NSString *urlString = [NSString stringWithFormat:@"socket://%s", self.host];
	NSURL *url = [NSURL URLWithString:urlString];
	CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)url, port, &readStream, &writeStream);
	
	inputStream = (NSInputStream *)readStream;
	outputStream = (NSOutputStream *)writeStream;
	
	[self.inputStream open];
	[self.outputStream open];
	
	self.inputStream.delegate = self;
	self.outputStream.delegate = self;
	
	[self.inputStream scheduleInRunLoop:[self currentRunLoop] forMode:NSDefaultRunLoopMode];
	[self.outputStream scheduleInRunLoop:[self currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)write:(NSString *)string {
	
}

- (NSString *)read {
	return nil;
}

@end
