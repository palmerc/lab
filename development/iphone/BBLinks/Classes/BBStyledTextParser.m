//
//  BBStyledTextParser.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledTextParser.h"
#import "BBStyledTextNode.h"
#import "BBStyledLinkNode.h"
#import "BBStyledLineBreakNode.h"

@implementation BBStyledTextParser

@synthesize rootNode = _rootNode;
@synthesize parseLineBreaks = _parseLineBreaks;
@synthesize parseURLs = _parseURLs;

- (void)addNode:(BBStyledNode *)node {
	if (!_rootNode) {
		_rootNode = [node retain];
		_lastNode = node;
	} else if (_topElement) {
		[_topElement addChild:node];
	} else {
		// Set the previous lastNode to point to the new node the make the lastNode the new one.
		_lastNode.nextSibling = node;
		_lastNode = node;
	}
}

- (void)parseURLs:(NSString *)string {
	NSInteger stringIndex = 0;
	
	while (stringIndex < string.length) {
		NSRange searchRange = NSMakeRange(stringIndex, string.length - stringIndex);
		NSRange startRange = [string rangeOfString:@"http://" options:NSCaseInsensitiveSearch range:searchRange];
		
		if (startRange.location == NSNotFound) {
			NSString *text = [string substringWithRange:searchRange];
			BBStyledTextNode *node = [[[BBStyledTextNode alloc] initWithText:text] autorelease];
			[self addNode:node];
		} else {
			NSRange beforeRange = NSMakeRange(searchRange.location, startRange.location - searchRange.location);
			if (beforeRange.length) {
				NSString *text = [string substringWithRange:beforeRange];
				BBStyledTextNode *node = [[[BBStyledTextNode alloc] initWithText:text] autorelease];
				[self addNode:node];
			}
			
			NSRange subSearchRange = NSMakeRange(startRange.location, string.length - startRange.location);
			NSRange endRange = [string rangeOfString:@" " options:NSCaseInsensitiveSearch range:subSearchRange];
			if (endRange.location == NSNotFound) {
				NSString *URL = [string substringWithRange:subSearchRange];
				BBStyledLinkNode *node = [[[BBStyledLinkNode alloc] initWithText:URL] autorelease];
				node.URL = URL;
				[self addNode:node];
				
				break;
			} else {
				NSRange URLRange = NSMakeRange(startRange.location, endRange.location - startRange.location);
				NSString *URL = [string substringWithRange:URLRange];
				BBStyledLinkNode *node = [[[BBStyledLinkNode alloc] initWithText:URL] autorelease];
				node.URL = URL;
				[self addNode:node];
				stringIndex = endRange.location;
			}
			
			
		}
	}
}

- (void)parseText:(NSString *)string {
	NSCharacterSet *newLines = [NSCharacterSet newlineCharacterSet];
	NSInteger stringIndex = 0;
	NSInteger length = string.length;
	
	if (_parseLineBreaks) {
		while (1) {
			NSRange searchRange = NSMakeRange(stringIndex, length - stringIndex);
			NSRange range = [string rangeOfCharacterFromSet:newLines options:0 range:searchRange];
			if (range.location != NSNotFound) {
				NSRange textRange = NSMakeRange(stringIndex, range.location - stringIndex);
				NSString *substr = [string substringWithRange:textRange];
				[self parseURLs:substr];
				
				BBStyledLineBreakNode *br = [[[BBStyledLineBreakNode alloc] init] autorelease];
				[self addNode:br];
				
				stringIndex = stringIndex + substr.length + 1;
			} else {
				NSString *substr = [string substringFromIndex:stringIndex];
				[self parseURLs:substr];
				break;
			}
		}
	} else if (_parseURLs) {
		[self parseURLs:string];
	} else {
		BBStyledTextNode *node = [[[BBStyledTextNode alloc] initWithText:string] autorelease];
		[self addNode:node];
	}
}

- (void)dealloc {
	[super dealloc];
}

@end
