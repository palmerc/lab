//
//  BBStyledLinkNode.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledLinkNode.h"
#import "BBStyledInline.h"
#import "BBStyledNode.h"

@implementation BBStyledLinkNode
@synthesize URL = _URL;
@synthesize highlighted = _highlighted;

- (id)init {
	self = [self initWithText:nil URL:nil next:nil];
	if (self != nil) {
	}
	
	return self;
}

- (id)initWithURL:(NSString *)URL {
	self = [self initWithText:nil URL:URL next:nil];
	if (self != nil) {
	}
	
	return self;
}

- (id)initWithURL:(NSString *)URL next:(BBStyledNode *)nextSibling {
	self = [self initWithText:nil URL:URL next:nextSibling];
	if (self != nil) {
	}
	
	return self;
}

- (id)initWithText:(NSString *)text URL:(NSString *)URL next:(BBStyledNode *)nextSibling {
	self = [super initWithText:text next:nextSibling];
	if (self != nil) {
		self.URL = URL;
	}
	
	return self;
}

- (NSString *)description {
	return [NSString stringWithFormat:@"<%@>", _firstChild];
}

- (void)dealloc {
	[_URL release];
	[super dealloc];
}

@end
