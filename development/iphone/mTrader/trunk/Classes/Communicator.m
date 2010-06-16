//
//  Communicator.m
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//
#import "Communicator.h"
#import "NSMutableArray+QueueAdditions.h"

@implementation Communicator

@synthesize delegate = _delegate;
@synthesize host = _host;
@synthesize port = _port;
@synthesize inputStream = _inputStream;
@synthesize outputStream = _outputStream;
@synthesize lineBuffer = _lineBuffer;
@synthesize mutableDataBuffer = _mutableDataBuffer;
@synthesize isConnected = _isConnected;
@synthesize bytesReceived = _bytesReceived;
@synthesize bytesSent = _bytesSent;

#define DEBUG 0

#pragma mark -
#pragma mark Initialization, Description, and Cleanup

/**
 * Setup of the basic object
 *
 */

- (id)initWithSocket:(NSString *)host onPort:(NSInteger)port {
	self = [super init];
	if (self != nil) {
		// Subscribe to notifications from Rechability regarding network status changes
		_host = [host retain];
		_port = port;
		_inputStream = nil;
		_outputStream = nil;
		_isConnected = NO;
		
		_lineBuffer = nil;
		_mutableDataBuffer = nil;
		
		_bytesReceived = 0;
		_bytesSent = 0;
	}
	return self;
}

- (NSString *)description {
	return [NSString stringWithFormat:@"Network connection: Connected to %@ on port %d", self.host, self.port];
}

#pragma mark Sending and Receiving
/**
 * stream: handleEvent: gets called whenever bytes are available
 *
 */
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)streamEvent {
	switch (streamEvent) {
		case NSStreamEventNone:
#if DEBUG
			NSLog(@"Communicator - NSStreamEventNone");
#endif
			break;
		case NSStreamEventOpenCompleted:
			if (self.delegate && [self.delegate respondsToSelector:@selector(connected)]) {
				[self.delegate connected];
			}
			self.isConnected = YES;

			break;
		case NSStreamEventHasBytesAvailable:
			[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
			[NSThread detachNewThreadSelector:@selector(readAvailableBytes:) toTarget:self withObject:(NSInputStream *)aStream];
			[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
			
			break;
		case NSStreamEventHasSpaceAvailable:
#if DEBUG
			NSLog(@"Communicator - Has Space Available");
#endif
			break;
		case NSStreamEventErrorOccurred:
		{
#if DEBUG
			NSError *theError = [aStream streamError];
			NSLog(@"Communicator - NSStreamEventErrorOccurred: %@ %@", [theError code], [theError localizedDescription]);
#endif
			if (self.delegate && [self.delegate respondsToSelector:@selector(disconnected)]) {
				[self.delegate disconnected];
			}			
			self.isConnected = NO;
		}
			break;
		case NSStreamEventEndEncountered:
			if (self.delegate && [self.delegate respondsToSelector:@selector(disconnected)]) {
				[self.delegate disconnected];
			}
			self.isConnected = NO;
			break;
		default:
#if DEBUG
			NSLog(@"Communicator - NSStream handleEvent - default");
#endif
			break;
	}
}

#pragma mark -
#pragma mark Threads For Reading and Writing
- (void)readAvailableBytes:(NSInputStream *)aStream {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
#if DEBUG
	NSLog(@"%@", aStream);
#endif
	uint8_t buffer[4096];
	bzero(buffer, sizeof(buffer));
	unsigned int len = 0;
	
	len = [aStream read:buffer maxLength:sizeof(buffer)];
	_bytesReceived += len;
	if (len > 0) {
		NSData *data = [NSData dataWithBytes:(const void *)buffer length:len];
		[self performSelectorOnMainThread:@selector(dataReceived:) withObject:data waitUntilDone:YES];
	}
		
	[pool release];
}

- (void)sendBytes:(NSString *)aString {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	if ([self.outputStream hasSpaceAvailable]) {
#if DEBUG
		NSLog(@"->> {%@}", aString);
#endif
		NSData *data = [aString dataUsingEncoding:NSISOLatin1StringEncoding];
		
		// Convert it to a C-string
		int bytesRemaining = [data length];
		uint8_t *theBytes = (uint8_t *)[data bytes];
		
		while (0 < bytesRemaining) {
			int bytesWritten = 0;
			bytesWritten = [self.outputStream write:theBytes maxLength:bytesRemaining];
			_bytesSent += bytesWritten;
			bytesRemaining -= bytesWritten;
			theBytes += bytesWritten;
		}
	}
	
	[pool release];
}

#pragma mark -
#pragma mark Handle Data Returned From Thread

- (void)dataReceived:(NSData *)dataBlock {
	if (_lineBuffer == nil) {
		_lineBuffer = [[NSMutableArray alloc] init];
	}
	
	const char *bytes = [dataBlock bytes];
	
	char previousByte = 0;
	char currentByte = 0;
	for (int i = 0; i < [dataBlock length]; i++) {
		if (_mutableDataBuffer == nil) {
			_mutableDataBuffer = [[NSMutableData alloc] init];
		}
		
		[self.mutableDataBuffer appendBytes:&bytes[i] length:1];
		
		currentByte = bytes[i];
		if ((previousByte == '\r' && currentByte == '\r') || (previousByte == '\r' && currentByte == '\n')) {
			NSData *aLine = self.mutableDataBuffer;
			[self.lineBuffer enQueue:aLine];
			
			if (self.delegate && [self.delegate respondsToSelector:@selector(dataReceived)]) {
				[self.delegate dataReceived];
			}
			
#if DEBUG
			NSLog(@"Length: %d, Content: %@", [aLine length], aLine);
#endif
			[_mutableDataBuffer release];
			_mutableDataBuffer = nil;
		}
		
		previousByte = currentByte;
	}
}

#pragma mark -
#pragma mark Public Read and Write Methods

/**
 * Send data out on the connection as an ISO Latin 1 string
 *
 */
- (void)writeString:(NSString *)aString {
	[NSThread detachNewThreadSelector:@selector(sendBytes:) toTarget:self withObject:aString];
}

/**
 * Read a line out of our queue
 *
 */
- (NSData *)readLine {
	NSData *oneLine = nil;
	if ([self.lineBuffer count] > 0) {
		oneLine = [self.lineBuffer deQueue];
	}
	return oneLine;
}

#pragma mark Connection Startup and Shutdown
/**
 * Setup the network connection and add ourselves to the run loop.
 * This includes monitoring the status of the network connection.
 *
 */
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
		
	[self.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
	[self.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
}

/**
 * Shut down the network connection.
 *
 */
- (void)stopConnection {
	[self.inputStream close];
	[self.outputStream close];
	
	[self.inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
	[self.outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
	
	[self.inputStream release];
	[self.outputStream release];
	
	self.inputStream = nil;
	self.outputStream = nil;
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_host release];
	[_inputStream release];
	[_outputStream release];
	[_lineBuffer release];
	[_mutableDataBuffer release];
	
	[super dealloc];
}

@end
