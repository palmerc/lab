//
//  LineOrientedCommunication.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG_INCOMING 0
#define DEBUG_OUTGOING 0
#define DEBUG_LEFTOVERS 0

#import "LineOrientedCommunication.h"

#import "NSData+StringAdditions.h"

@interface LineOrientedCommunication ()
- (void)processDataBuffer;
@end


@implementation LineOrientedCommunication
@synthesize dataDelegate = _dataDelegate;

#pragma mark -
#pragma mark Initialization

- (id)init {
	self = [super init];
	if (self != nil) {
		_dataBuffer = nil;
	}
	return self;
}


#pragma mark -
#pragma mark CommunicatorDataDelegate methods

- (void)receivedData:(NSData *)data {
	if (_dataBuffer == nil) {
		_dataBuffer = [[NSMutableData alloc] init];
	}

	[_dataBuffer appendData:data];
	
	if ([_dataBuffer length] > 0) {
		[self processDataBuffer];	
	}
}


#pragma mark -
#pragma mark Data to Lines Methods

- (void)processDataBuffer {
	const char* characters = [_dataBuffer mutableBytes];
	
	NSRange lineRange;
	lineRange.location = 0;
	lineRange.length = 0;
	
	NSRange dataBufferRange;
	dataBufferRange.location = 0;
	dataBufferRange.length = 0;
	
	char current = ' ';
	char previous = ' ';
	// Find all the lines ending with CRLF
	for (int i = 0; i < [_dataBuffer length]; i++) {
		current = characters[i];
		if (previous == '\r' && current == '\n') {
			lineRange.length = (i + 1) - lineRange.location;
			NSData *aDataLine = [_dataBuffer subdataWithRange:lineRange];
			
			if (self.dataDelegate && [self.dataDelegate respondsToSelector:@selector(receivedDataLine:)]) {
				[self.dataDelegate receivedDataLine:aDataLine];
			}
			
#if DEBUG_INCOMING
			NSLog(@"LineOrientedCommunication");
			NSLog(@"Raw: %@", aDataLine);
			NSLog(@"Text: %@", [aDataLine string]);
#endif	
			
			// Advance to the next character
			lineRange.location = i + 1;
			dataBufferRange.location = i + 1;
		}
		
		previous = current;
	}
	
	// Stuff the leftovers back into the buffer at the beginning
	dataBufferRange.length = [_dataBuffer length] - lineRange.location;
	
	NSData *leftovers = [_dataBuffer subdataWithRange:dataBufferRange];
	
#if DEBUG_LEFTOVERS
	NSLog(@"LineOrientedCommunication");
	NSLog(@"Raw: %@", leftovers);
	NSLog(@"Text: %@", [leftovers string]);
#endif
	
	[_dataBuffer release];
	_dataBuffer = nil;
	
	_dataBuffer = [[NSMutableData dataWithBytes:[leftovers bytes] length:[leftovers length]] retain];
}

- (void)dealloc {
	[_dataBuffer release];
	
	[super dealloc];
}

@end
