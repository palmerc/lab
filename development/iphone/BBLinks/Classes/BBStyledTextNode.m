//
//  BBStyledTextNode.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledTextNode.h"

@implementation BBStyledTextNode
@synthesize text = _text;

- (id)initWithText:(NSString *)text {
	self = [self initWithText:text next:nil];
	if (self != nil) {
	}
	
	return self;
}

- (id)initWithText:(NSString *)text next:(BBStyledNode *)nextSibling {
	self = [super initWithNextSibling:nextSibling];
	if (self != nil) {
		self.text = text;
	}
	
	return self;	
}

- (NSString *)outerText {
	return _text;
}

- (NSString *)outerHTML {
	if (_nextSibling) {
		return [NSString stringWithFormat:@"%@%@", _text, _nextSibling.outerHTML];
	} else {
		return _text;
	}
	return;
}

- (void)description {
	return _text;
}

- (void)dealloc {
	[_text release];
	
	[super dealloc];
}

@end
