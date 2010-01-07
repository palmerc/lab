//
//  Feed.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Feed.h"


@implementation Feed
@synthesize number = _number;
@synthesize feedDescription = _feedDescription;
@synthesize code = _code;
@synthesize symbols = _symbols;

-(id)init {
	self = [super init];
	if (self != nil) {
		_number = nil;
		_feedDescription = nil;
		_code = nil;
		_symbols = nil;
	}
	return self;
}

-(void)dealloc {
	[self.number release];
	[self.feedDescription release];
	[self.code release];
	[self.symbols release];
	[super dealloc];
}

-(NSString *)description {
	return [NSString stringWithFormat:@"(Number: %@, Description: %@, Code: %@)", self.number, self.feedDescription, self.code];
}

- (void)addSymbol:(Symbol *)symbol {
	if (self.symbols == nil) {
		self.symbols = [[NSMutableArray alloc] init];
	}
	[self.symbols addObject:symbol];
}

- (void)deleteSymbol:(Symbol *)symbol {
	assert(_symbols != nil);
	[self.symbols removeObject:symbol];
}

@end
