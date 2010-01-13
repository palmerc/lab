//
//  Communicator.m
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Bird And Bear Productions. All rights reserved.
//

#import "Communicator.h"
#import "Queue.h"

@implementation Communicator

@synthesize delegate;
@synthesize host = _host;
@synthesize port = _port;
@synthesize inputStream = _inputStream;
@synthesize outputStream = _outputStream;
@synthesize dataBuffer = _dataBuffer;
@synthesize lineBuffer = _lineBuffer;
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
		_dataBuffer = nil;
		_isConnected = NO;
		
		_lineBuffer = [[NSMutableArray alloc] init];
	}
	return self;
}

- (void)dealloc {
	[self.lineBuffer release];
	
	[super dealloc];
}

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)streamEvent {
	switch (streamEvent) {
		case NSStreamEventNone:
			break;
		case NSStreamEventOpenCompleted:
			break;
		case NSStreamEventHasBytesAvailable:
		{
			uint8_t oneByte;
			int bytesRead = 0;
			
			if (self.dataBuffer == nil) {
				self.dataBuffer = [[NSMutableData alloc] init];
			}
		
			bytesRead = [self.inputStream read:&oneByte maxLength:1];
			if (bytesRead == 1) {
				[self.dataBuffer appendBytes:&oneByte length:1];
				
				if (oneByte == '\n') {
					NSData *oneLine = [NSData dataWithData:self.dataBuffer];
					[self.lineBuffer enQueue:oneLine];
					if (self.delegate != nil && [self.delegate respondsToSelector:@selector(dataReceived)]) {
						[self.delegate dataReceived];
					}
					[self.dataBuffer release];
					self.dataBuffer = nil;
				}
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

- (void)writeString:(NSString *)string {
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
			 
- (NSData *)readLine {
	NSData *oneLine = nil;
	// dequeue strings until I find a \n
	if ([self.lineBuffer count] > 0) {
		oneLine = [self.lineBuffer deQueue];
	}
	return oneLine;
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

@end
