//
//  Communicator.m
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Bird And Bear Productions. All rights reserved.
//

#import "Communicator.h"


@implementation Communicator

@synthesize host = _host;
@synthesize port = _port;
@synthesize inputStream = _inputStream;
@synthesize outputStream = _outputStream;
@synthesize inputBuffer = _inputBuffer;
@synthesize outputBuffer = _outputBuffer;
@synthesize bytesRead = _bytesRead;
@synthesize isConnected = _isConnected;
@synthesize byteIndex = _byteIndex;

- (NSString *)description {
	return [NSString stringWithFormat:@"Network connection: Connected to %@ on port %d", self.host, self.port];
}

- (id)init {
	return [self initWithSocket:nil port:0];
}

- (id)initWithSocket:(NSString *)host port:(NSInteger)port {
	self = [super init];
	if (self != nil) {
		_host = host;
		_port = port;
		_inputStream = nil;
		_outputStream = nil;
		_bytesRead = [NSNumber numberWithInt:0];
		_isConnected = NO;
	}
	return self;
}

- (void)dealloc {
	[super dealloc];
}

- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode {
	switch(eventCode) {
		case NSStreamEventNone:
			// ignore
			break;
		case NSStreamEventOpenCompleted:
			// Connection opened
			NSLog(@"Network connection: opened");
			break;
		case NSStreamEventHasBytesAvailable:
			NSLog(@"Reading");
			// Receiving data - inputStream
			assert(stream == self.inputStream);
			[self read];
			break;
		case NSStreamEventHasSpaceAvailable:
			// Sending data - outputStream
			assert(stream == self.outputStream);
			NSLog(@"Writing");
			break;
		case NSStreamEventErrorOccurred:
			// Stream error
			break;
		case NSStreamEventEndEncountered:
			// Connection ended
			NSLog(@"Network connection: ended");
			break;
		default:
			assert(NO);
	}
}

- (void)startConnection {
	CFWriteStreamRef writeStream;
	CFReadStreamRef readStream;
	
	NSString *urlString = [NSString stringWithFormat:@"socket://%@", self.host];
	NSURL *url = [NSURL URLWithString:urlString];
	CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)[url host], self.port, &readStream, &writeStream);
	
	self.inputStream = (NSInputStream *)readStream;
	self.outputStream = (NSOutputStream *)writeStream;
	
	[self.inputStream open];
	[self.outputStream open];
	
	self.inputStream.delegate = self;
	self.outputStream.delegate = self;
	
	[self.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[self.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	
	self.isConnected = YES;
}

- (void)stopConnection {	
	[self.inputStream close];
	[self.outputStream close];
	
	[self.inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[self.outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	
	[self.inputStream release];
	[self.outputStream release];
	
	self.inputStream = nil;
	self.outputStream = nil;
	
	self.isConnected = NO;
}

- (void)write:(NSData *)data {
	// Allocate a NSMutableData buffer
	if (!self.outputBuffer) {
		self.outputBuffer = [[NSMutableData alloc] init];
	}
	
	// Append the output
	[self.outputBuffer appendData:data];
	// Convert it to a C-string
	uint8_t *readBytes = (uint8_t *)[self.outputBuffer mutableBytes];
	
	readBytes += self.byteIndex;
	int data_len = [self.outputBuffer length];
	
	unsigned int len = ( (data_len - self.byteIndex >= 1024) ? 1024 : (data_len - self.byteIndex) );
	uint8_t buf[len];
	
	memcpy(buf, readBytes, len);
	
	len = [self.outputStream write:(const uint8_t *)buf maxLength:len];
	
	self.byteIndex += len;
}

- (NSData *)read {
	if (!self.inputBuffer) {
		self.inputBuffer = [[NSMutableData alloc] init];
	}
	
	uint8_t buf[1024];
	unsigned int len = 0;
	len = [(NSInputStream *)self.inputStream read:buf maxLength:sizeof(buf)];
	
	if (len) {
		[self.inputBuffer appendBytes:(const void *)buf length:len];
		self.bytesRead = [NSNumber numberWithInt:[self.bytesRead intValue] + len];
	}
	NSLog(@"%@", [NSString stringWithCString:[self.inputBuffer mutableBytes] encoding:NSASCIIStringEncoding]);
	
	[self.inputBuffer release];
	self.inputBuffer = nil;
	return nil;
}

- (void)test {
	NSString *login = @"\r\n\r\nAction: login\r\nAuthorization: cameron/Ct1gg3rR\r\n";
	[self write:[login dataUsingEncoding:NSASCIIStringEncoding]];
}

@end
