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
@synthesize dataBuffer = _dataBuffer;
@synthesize lineBuffer = _lineBuffer;
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
		
		_lineBuffer = [[NSMutableArray alloc] init];
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
			NSLog(@"NSStreamEventNone");
			break;
		case NSStreamEventOpenCompleted:
			if (self.delegate && [self.delegate respondsToSelector:@selector(connected)]) {
				[self.delegate connected];
			}
			self.isConnected = YES;

			break;
		case NSStreamEventHasBytesAvailable:
		{
			uint8_t currentByte;
			int bytesRead = 0;
			
			if (_dataBuffer == nil) {
				_dataBuffer = [[NSMutableData alloc] init];
			}
		
			bytesRead = [self.inputStream read:&currentByte maxLength:1];
			if (bytesRead == 1) {
				[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;

				[self.dataBuffer appendBytes:&currentByte length:1];
				
				if ((previousByte == '\r' && currentByte == '\r') || (previousByte == '\r' && currentByte =='\n')) {
					NSData *oneLine = [NSData dataWithData:self.dataBuffer];
					[self.lineBuffer enQueue:oneLine];
					if (self.delegate && [self.delegate respondsToSelector:@selector(dataReceived)]) {
						[self.delegate dataReceived];
					}
					[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
					[_dataBuffer release];
					_dataBuffer = nil;
				}
				
				previousByte = currentByte;
			}
		}
			break;
		case NSStreamEventHasSpaceAvailable:
			break;
		case NSStreamEventErrorOccurred:
			if (self.delegate && [self.delegate respondsToSelector:@selector(disconnected)]) {
				[self.delegate disconnected];
			}			
			self.isConnected = NO;
			break;
		case NSStreamEventEndEncountered:
			if (self.delegate && [self.delegate respondsToSelector:@selector(disconnected)]) {
				[self.delegate disconnected];
			}
			self.isConnected = NO;
			break;
		default:
			NSLog(@"default");
	}
}

/**
 * Send data out on the connection as an ISO Latin 1 string
 *
 */
- (void)writeString:(NSString *)string {
	if ([self.outputStream hasSpaceAvailable]) {
		NSLog(@"%@", string);
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

/**
 * Read a line out of our queue
 *
 */
- (NSData *)readLine {
	NSData *oneLine = nil;
	// dequeue strings until I find a \n
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
	
	[super dealloc];
}

@end
