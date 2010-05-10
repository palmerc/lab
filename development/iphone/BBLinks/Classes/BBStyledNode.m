//
//  BBStyledNode.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledNode.h"

@implementation BBStyledNode

@synthesize nextSibling = _nextSibling;
@synthesize parentNode = _parentNode;

- (id)init {
	self = [self initWithNextSibling:nil];
	if (self != nil) {
	}
	
	return self;	
}

- (id)initWithNextSibling:(BBStyledNode *)nextSibling {
	self = [super init];
	if (self != nil) {
		self.nextSibling = nextSibling;
	}
	
	return self;
}

- (void)setNextSibling:(BBStyledNode *)node {
	if (node != _nextSibling) {
		[_nextSibling release];
		_nextSibling = [node retain];
		node.parentNode = _parentNode;
	}
}

- (NSString *)outerText {
	return @"";
}

- (NSString *)outerHTML {
	if (_nextSibling) {
		return _nextSibling.outerHTML;
	} else {
		return @"";
	}
}

- (id)ancestorOrSelfWithClass:(Class)c {
	if ([self isKindOfClass:c]) {
		return self;
	} else {
		return [_parentNode ancestorOrSelfWithClass:c];
	}
}

- (void)performDefaultAction {
}

- (void)dealloc {
	[_nextSibling release];
	
	[super dealloc];
}

@end
