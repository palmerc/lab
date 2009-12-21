//
//  Communicator.m
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Bird And Bear Productions. All rights reserved.
//

#import "Communicator.h"
#import "queue.h"

@implementation Communicator

@synthesize delegate;
@synthesize host = _host;
@synthesize port = _port;
@synthesize inputStream = _inputStream;
@synthesize outputStream = _outputStream;
@synthesize isConnected = _isConnected;

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
		_isConnected = NO;
		
		theQueue = [[queue alloc] init];
	}
	return self;
}

- (void)dealloc {
	[super dealloc];
}

- (void)write:(NSString *)string {
	if (!self.isConnected) {
		[self startConnection];
	}
	
	NSData *data = [string dataUsingEncoding:NSASCIIStringEncoding];
	
	// Convert it to a C-string
	int bytesRemaining = [data length];
	uint8_t *theBytes = (uint8_t *)[data bytes];
	
	while (0 < bytesRemaining) {
		int bytesWritten = 0;
		bytesWritten = [self.outputStream write:theBytes maxLength:bytesRemaining];
		bytesRemaining -= bytesWritten;
		theBytes += bytesWritten;
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

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)streamEvent {
	switch (streamEvent) {
		case NSStreamEventNone:
			break;
		case NSStreamEventOpenCompleted:
			break;
		case NSStreamEventHasBytesAvailable:
			{
				uint8_t aByte;
				int bytesRead = 0;
				NSMutableData *inputBuffer = [[NSMutableData alloc] init];
				
				bytesRead = [self.inputStream read:&aByte maxLength:1];
				if (bytesRead == 1) {
					[inputBuffer appendBytes:&aByte length:1];
				}
				
				if (aByte == '\n') {
					NSString *aString = [[NSString alloc] initWithData:inputBuffer encoding:NSASCIIStringEncoding];
					
					if (self.delegate != nil && [self.delegate respondsToSelector:@selector(dataReceived)]) {
						[self.delegate dataReceived];
					}
					[queue inQueue:aString];
					
					[aString release];
					
					[inputBuffer release];
					inputBuffer = nil;
				}
			}
			break;
		case NSStreamEventHasSpaceAvailable:
			break;
		case NSStreamEventErrorOccurred:
			break;
		case NSStreamEventEndEncountered:
			break;
		default:
			assert(NO);
	}
}

@end
