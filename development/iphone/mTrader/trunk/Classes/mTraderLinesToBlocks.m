//
//  mTraderLinesToBlocks.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG_BLOCK 1


#import "mTraderLinesToBlocks.h"

#import "NSMutableArray+QueueAdditions.h"
#import "NSData+StringAdditions.h"

@interface mTraderLinesToBlocks ()
- (void)processLineBuffer:(NSData *)data;
@end


@implementation mTraderLinesToBlocks
@synthesize dataDelegate = _dataDelegate;

- (id)init {
	self = [super init];
	if (self != nil) {
		_blockBuffer = nil;
	}
	return self;
}

- (void)receivedDataLine:(NSData *)data {	
	[self processLineBuffer:data];
}

- (void)processLineBuffer:(NSData *)data {
	if (_blockBuffer == nil) {
		_blockBuffer = [[NSMutableArray alloc] init];
	}
	
	char CRCR[] = "\r\r";
	
	NSData *currentLine = data;
	
	const char *bytes = [currentLine bytes];
	if (strcmp(bytes, CRCR) == 0) {
#if DEBUG_BLOCK
		NSLog(@"mTraderLinesToBlocks: Block contains %d lines", [_blockBuffer count]);
		for (NSData *data in _blockBuffer) {
			NSLog(@"Raw: %@", data);
			NSLog(@"Text: %@", [data string]);
		}
#endif
		if (self.dataDelegate && [self.dataDelegate respondsToSelector:@selector(receivedDataBlock:)]) {			
			NSArray *aBlock = [NSArray arrayWithArray:_blockBuffer];
			[_blockBuffer release];
			_blockBuffer = nil;
			[self.dataDelegate receivedDataBlock:aBlock];
		}
	} else {
		[_blockBuffer enQueue:currentLine];
	}
}

- (void)dealloc {
	[_blockBuffer release];
	
	[super dealloc];
}

@end
