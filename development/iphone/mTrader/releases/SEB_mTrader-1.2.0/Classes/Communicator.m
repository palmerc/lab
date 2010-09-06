//
//  Communicator.m
//  Simple Asynchronous Networking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG_INCOMING 0
#define DEBUG_OUTGOING 0
#define DEBUG_HANDLEEVENT 0

#define BUFFER_SIZE 2048


#import "Communicator.h"

#import "NSData+StringAdditions.h"

@interface Communicator ()
- (void)dataReceived:(NSInputStream *)aStream;
- (void)sendAvailableBytes:(NSOutputStream *)aStream;
@end


@implementation Communicator
@synthesize statusDelegate = _statusDelegate;
@synthesize dataDelegate = _dataDelegate;
@synthesize url = _url;
@synthesize port = _port;
@synthesize bytesReceived = _bytesReceived;
@synthesize bytesSent = _bytesSent;
@synthesize tlsEnabled = _tlsEnabled;


#pragma mark -
#pragma mark Initialization

- (id)init {
	self = [super init];
	if (self != nil) {
		_url = nil;
		_port = 0;
		_inputStream = nil;
		_outputStream = nil;
		_inboundBuffer = nil;
		_outboundBuffer = nil;
		
		_bytesReceived = 0;
		_bytesSent = 0;
		
		_tlsEnabled = NO;
	}
	return self;
}

- (NSString *)description {
	return [NSString stringWithFormat:@"Socket connection to %@ on port %d", self.url, self.port];
}

#pragma mark -
#pragma mark NSStreamDelegate

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)streamEvent {
#if DEBUG_HANDLEEVENT
	NSError *error = nil;
	NSInteger code;
	NSString *description = nil;	
#endif
	
	switch (streamEvent) {
		case NSStreamEventNone:
#if DEBUG_HANDLEEVENT
			NSLog(@"Communicator: NSStreamEventNone");
			NSLog(@"%@", [aStream streamError]);
#endif			
			break;
		case NSStreamEventOpenCompleted:
#if DEBUG_HANDLEEVENT
			NSLog(@"Communicator: NSStreamEventOpenCompleted");
#endif
			if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(connect)]) {
				[self.statusDelegate connect];
			}
			break;
		case NSStreamEventHasBytesAvailable:
#if DEBUG_HANDLEEVENT
			NSLog(@"Communicator: NSStreamEventHasBytesAvailable");
#endif
			[self dataReceived:(NSInputStream *)aStream];
			break;
		case NSStreamEventHasSpaceAvailable:
#if DEBUG_HANDLEEVENT
			NSLog(@"Communicator: NSStreamEventHasSpaceAvailable");
#endif
			[self sendAvailableBytes:(NSOutputStream *)aStream];
			break;
		case NSStreamEventErrorOccurred:
#if DEBUG_HANDLEEVENT
			NSLog(@"Communicator: NSStreamEventErrorOccurred");
			error = [aStream streamError];
			code = [error code];
			description = [error localizedDescription];
			NSLog(@"Error code: %i, description: %@", code, description);
#endif
			if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(disconnect)]) {
				[self.statusDelegate disconnect];
			}			
			break;
		case NSStreamEventEndEncountered:
#if DEBUG_HANDLEEVENT
			NSLog(@"Communicator: NSStreamEventEndEncountered");
			error = [aStream streamError];
			code = [error code];
			description = [error localizedDescription];
			NSLog(@"Stream: %@", aStream);
			NSLog(@"Error code: %i, description: %@", code, description);
#endif
			if (self.statusDelegate && [self.statusDelegate respondsToSelector:@selector(disconnect)]) {
				[self.statusDelegate disconnect];
			}
			
			[self stopConnection];
			
			break;
		default:
#if DEBUG_HANDLEEVENT
			NSLog(@"Communicator: Handle Event Default");
#endif		
			break;
	}
	
}

#pragma mark -
#pragma mark Private send and receive methods

- (void)dataReceived:(NSInputStream *)aStream {
	[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
	
	if (_inboundBuffer == nil) {
		_inboundBuffer = [[NSMutableData alloc] init];
	}
	
	uint8_t buffer[BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));
	NSInteger len = 0;
			
	len = [aStream read:buffer maxLength:sizeof(buffer)];
	_bytesReceived += len;
	if (len > 0) {
		[_inboundBuffer appendBytes:buffer length:len];
	}
#if DEBUG_INCOMING	
	NSLog(@"Communicator: RECEIVED %d bytes", len);
	NSLog(@"Raw: %@", _inboundBuffer);
	NSLog(@"Text: %@", [_inboundBuffer string]);
#endif
	
	
	if (self.dataDelegate && [self.dataDelegate respondsToSelector:@selector(receivedData:)]) {
		[self.dataDelegate receivedData:_inboundBuffer];
	}
	[_inboundBuffer release];
	_inboundBuffer = nil;
	
	[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
}

- (void)sendAvailableBytes:(NSOutputStream *)aStream {
	if (_outboundBuffer == nil) {
		return;
	}
	
	NSUInteger bytesRemaining = [_outboundBuffer length];
	
	if ( bytesRemaining > 0 ) {	
		// Convert it to a C-string
		uint8_t *theBytes = (uint8_t *)[_outboundBuffer bytes];
		
		while (0 < bytesRemaining) {
			int bytesWritten = 0;
			bytesWritten = [aStream write:theBytes maxLength:bytesRemaining];
			_bytesSent += bytesWritten;
			bytesRemaining -= bytesWritten;
#if DEBUG_OUTGOING
			NSData *outboundData = [NSData dataWithBytes:theBytes length:bytesWritten];			
			NSLog(@"Communicator: SENT %d bytes, REMAINING %d bytes", bytesWritten, bytesRemaining);
			NSLog(@"Raw: %@", outboundData);
			NSLog(@"Text: %@", [outboundData string]);
#endif
			
			theBytes += bytesWritten;
		}
		
		[_outboundBuffer release];
		_outboundBuffer = nil;
	}
}

#pragma mark -
#pragma mark Public send method

- (void)sendData:(NSData *)data {
	NSAssert(_outputStream != nil, @"Outputstream is nil");
	
	if (_outboundBuffer == nil) {
		_outboundBuffer = [[NSMutableData alloc] init];
	}
	
	[_outboundBuffer appendData:data];
	
	if ([_outputStream hasSpaceAvailable]) {
		[self sendAvailableBytes:_outputStream];
	}
}

#pragma mark -
#pragma mark Connection start and stop
- (void)startConnectionWithSocket:(NSURL *)url onPort:(NSUInteger)port {
	NSAssert(url != nil && port > 0, @"Starting connection failed");
	_url = [url retain];
	_port = port;
	
	CFWriteStreamRef writeStream;
	CFReadStreamRef readStream;
	
	CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)[url host], self.port, &readStream, &writeStream);
	
	_inputStream = (NSInputStream *)readStream;
	_outputStream = (NSOutputStream *)writeStream;
	
	if (_tlsEnabled) {
		[_inputStream setProperty:NSStreamSocketSecurityLevelTLSv1 forKey:NSStreamSocketSecurityLevelKey];
		[_outputStream setProperty:NSStreamSocketSecurityLevelTLSv1 forKey:NSStreamSocketSecurityLevelKey];
	}
	
	[_inputStream open];
	[_outputStream open];
	
	_inputStream.delegate = self;
	_outputStream.delegate = self;
	
	[_inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[_outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)stopConnection {
	[_inputStream close];
	[_outputStream close];
	
	[_inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[_outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	
	[_inputStream release];
	[_outputStream release];
	
	_inputStream = nil;
	_outputStream = nil;
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_url release];
	[_inputStream release];
	[_outputStream release];
	[_inboundBuffer release];
	[_outboundBuffer release];
	
	[super dealloc];
}

@end
