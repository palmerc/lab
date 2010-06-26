//
//  Communicator.m
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

#define DEBUG_INCOMING 0
#define DEBUG_OUTGOING 0
#define DEBUG_LEFTOVERS 0
#define DEBUG_BLOCK 0
#define DEBUG_HANDLEEVENT 0

#import "Communicator.h"
#import "NSMutableArray+QueueAdditions.h"

@implementation Communicator

@synthesize delegate = _delegate;
@synthesize host = _host;
@synthesize port = _port;
@synthesize inputStream = _inputStream;
@synthesize outputStream = _outputStream;
@synthesize dataBuffer = _dataBuffer;
@synthesize lineBuffer = _lineBuffer;
@synthesize blockBuffer = _blockBuffer;
@synthesize isConnected = _isConnected;

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
		_dataBuffer = nil;
		_isConnected = NO;
		
		_lineBuffer = nil;
		_blockBuffer = nil;
	}
	return self;
}

- (NSString *)description {
	return [NSString stringWithFormat:@"Network connection: Connected to %@ on port %d", self.host, self.port];
}

#pragma mark Sending and Receiving
/**
 * The rest of the class handles the business of the Communicator
 *
 */

/**
 * stream: handleEvent: gets called whenever bytes are available
 *
 */
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)streamEvent {
	switch (streamEvent) {
		case NSStreamEventNone:
#if DEBUG_HANDLEEVENT
			NSLog(@"NSStreamEventNone");
#endif			
			break;
		case NSStreamEventOpenCompleted:
#if DEBUG_HANDLEEVENT
			NSLog(@"NSStreamEventOpenCompleted");
#endif
			if (self.delegate && [self.delegate respondsToSelector:@selector(connected)]) {
				[self.delegate connected];
			}
			self.isConnected = YES;

			break;
		case NSStreamEventHasBytesAvailable:
#if DEBUG_HANDLEEVENT
			NSLog(@"NSStreamEventHasBytesAvailable");
#endif
			[self dataReceived:(NSInputStream *)aStream];
			break;
		case NSStreamEventHasSpaceAvailable:
#if DEBUG_HANDLEEVENT
			NSLog(@"NSStreamEventHasSpaceAvailable");
#endif
			break;
		case NSStreamEventErrorOccurred:
#if DEBUG_HANDLEEVENT
			NSLog(@"NSStreamEventErrorOccurred");
#endif
			if (self.delegate && [self.delegate respondsToSelector:@selector(disconnected)]) {
				[self.delegate disconnected];
			}			
			self.isConnected = NO;
			break;
		case NSStreamEventEndEncountered:
#if DEBUG_HANDLEEVENT
			NSLog(@"NSStreamEventEndEncountered");
#endif
			if (self.delegate && [self.delegate respondsToSelector:@selector(disconnected)]) {
				[self.delegate disconnected];
			}
			self.isConnected = NO;
			break;
		default:
#if DEBUG_HANDLEEVENT
			NSLog(@"default");
#endif		
			break;
	}
	
}

- (void)dataReceived:(NSInputStream *)stream {
	[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
	
	uint8_t buffer[2048];
	bzero(buffer, sizeof(buffer));
	NSInteger len = 0;
	
	if (_dataBuffer == nil) {
		_dataBuffer = [[NSMutableData alloc] init];
	}
	
	len = [stream read:buffer maxLength:sizeof(buffer)];
	if (len > 0) {
		[self.dataBuffer appendBytes:buffer length:len];
	}
	
	[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
	[self processBuffer];
}

- (void)processBuffer {
	if (_lineBuffer == nil) {
		_lineBuffer = [[NSMutableArray alloc] init];
	}
	
	NSMutableData *dataBuffer = self.dataBuffer;
	const char* characters = [dataBuffer mutableBytes];
	
	NSRange lineRange;
	lineRange.location = 0;
	lineRange.length = 0;
	
	NSRange bufferRange;
	bufferRange.location = 0;
	bufferRange.length = 0;
	
	char current = ' ';
	char previous = ' ';
	for (int i = 0; i < [dataBuffer length]; i++) {
		current = characters[i];
		if ((previous == '\r' && current == '\r') || (previous == '\r' && current == '\n')) {
			lineRange.length = (i + 1) - lineRange.location;
			NSData *oneLine = [dataBuffer subdataWithRange:lineRange];
#if DEBUG_INCOMING
			NSString *lineString = [[NSString alloc] initWithData:oneLine encoding:NSISOLatin1StringEncoding];
			NSLog(@"\n<<<-%@", lineString);
			[lineString release];
#endif
			
			[self.lineBuffer enQueue:oneLine];

			// Advance to the next character
			lineRange.location = i + 1;
			bufferRange.location = i + 1;
		}
		
		previous = current;
	}
	
	bufferRange.length = [dataBuffer length] - lineRange.location;
	
	NSData *leftovers = [dataBuffer subdataWithRange:bufferRange];
#if DEBUG_LEFTOVERS
	NSString *lineString = [[NSString alloc] initWithData:leftovers encoding:NSISOLatin1StringEncoding];
	NSLog(@"\nLeftovers >>>%@<<<", lineString);
	[lineString release];
#endif
	[_dataBuffer release];
	_dataBuffer = nil;
	self.dataBuffer = [NSMutableData dataWithBytes:[leftovers bytes] length:[leftovers length]];
	
	[self processLines];
}

- (void)processLines {
	char CRCR[] = "\r\r";
	
	while ([self.lineBuffer count] > 0) {
		if (_blockBuffer == nil) {
			_blockBuffer = [[NSMutableArray alloc] init];
		}
		
		NSData *currentLine = [self.lineBuffer deQueue];
#if DEBUG_BLOCK
		NSString *lineString = [[NSString alloc] initWithData:currentLine encoding:NSISOLatin1StringEncoding];
		NSLog(@"queue: %@", lineString);
		[lineString release];
#endif
		
		const char *bytes = [currentLine bytes];
		if (strcmp(bytes, CRCR) == 0) {
			// Complete block
#if DEBUG_BLOCK
			NSLog(@"Shipping block");
#endif
			if (self.delegate && [self.delegate respondsToSelector:@selector(dataReceived:)]) {			
				[self.delegate dataReceived:self.blockBuffer];
			}
			self.blockBuffer = nil;
		} else {
			[self.blockBuffer enQueue:currentLine];
		}
	}	
}

/**
 * Send data out on the connection as an ISO Latin 1 string
 *
 */
- (void)writeString:(NSString *)string {
	if ([self.outputStream hasSpaceAvailable]) {
#if DEBUG_OUTGOING
		NSLog(@"\n->>>%@", string);
#endif
		NSData *data = [string dataUsingEncoding:NSISOLatin1StringEncoding];
		
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
	
	[self.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[self.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

/**
 * Shut down the network connection.
 *
 */
- (void)stopConnection {
	[self.inputStream close];
	[self.outputStream close];
	
	[self.inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	[self.outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	
	[self.inputStream release];
	[self.outputStream release];
	
	self.inputStream = nil;
	self.outputStream = nil;
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[self.host release];
	[self.inputStream release];
	[self.outputStream release];
	[_dataBuffer release];
	[_lineBuffer release];
	[_blockBuffer release];
	
	[super dealloc];
}

@end
