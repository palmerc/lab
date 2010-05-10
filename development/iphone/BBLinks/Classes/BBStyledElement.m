//
//  BBStyledElement.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledElement.h"

#import "BBStyledTextNode.h"

@implementation BBStyledElement
@synthesize firstChild = _firstChild;
@synthesize lastChild = _lastChild;
@synthesize className = _className;

- (id)init {
	self = [self initWithText:nil next:nil];
	if (self != nil) {
	}
	
	return self;
}

- (id)initWithText:(NSString *)text {
	self = [self initWithText:text next:nil];
	if (self != nil) {
	}
	 
	return self;
}
	 
- (id)initWithText:(NSString *)text next:(BBStyledNode *)nextSibling {
	self = [super initWithNextSibling:nextSibling];
	if (self != nil) {
		if (nil != text) {
			[self addChild:[[[BBStyledTextNode alloc] initWithText:text] autorelease]];
		}
	}
	
	return self;
}

- (NSString *)description {
	return [NSString stringWithFormat:@"%@", _firstChild];
}

- (NSString *)outerText {
	if (_firstChild) {
		NSMutableArray *strings = [NSMutableArray array];
		for (BBStyledNode *node = _firstChild; node; node.nextSibling) {
			[strings addObject:node.outerText];
		}
		
		return [strings componentsJoinedByString:@""];
	} else {
		return [super outerText];
	}
}

- (void)addChild:(BBStyledNode *)child {
	if (!_firstChild) {
		_firstChild = [child retain];
		_lastChild = [self findLastSibling:child];
	} else {
		_lastChild.nextSibling = child;
		_lastChild = [self findLastSibling:child];
	}
	
	child.parentNode = self;
}

- (void)addText:(NSString *)text {
	[self addChild:[[[BBStyledTextNode alloc] initWithText:text] autorelease]];
}

- (void)replaceChild:(BBStyledNode *)oldChild withChild:(BBStyledNode *)newChild {
	if (oldChild == _firstChild) {
		newChild.nextSibling = oldChild.nextSibling;
		oldChild.nextSibling = nil;
		newChild.parentNode = self;
		if (oldChild == _lastChild) {
			_lastChild = newChild;
		}
		[_firstChild release];
		_firstChild = [newChild retain];
	} else {
		BBStyledNode *node = _firstChild;
		while (node) {
			if (node.nextSibling == oldChild) {
				[oldChild retain];
				if (newChild) {
					newChild.nextSibling = oldChild.nextSibling;
					node.nextSibling = newChild;
				} else {
					node.nextSibling = oldChild.nextSibling;
				}
				oldChild.nextSibling = nil;
				newChild.parentNode = self;
				if (oldChild == _lastChild) {
					_lastChild = newChild ? newChild : node;
				}
				[oldChild release];
				break;
			}
			node = node.nextSibling;
		}
	}
}

- (void)dealloc {
	[_firstChild release];
	[_className release];
	
	[super dealloc];
}

@end
