//
//  BBStyledTextParser.h
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

@class BBStyledNode;
@class BBStyledElement;

@interface BBStyledTextParser : NSObject {
	BBStyledNode *_rootNode;
	BBStyledElement *_topElement;
	BBStyledNode *_lastNode;
	
	NSError *_parserError;
	
	NSMutableString *_chars;
	NSMutableArray *_stack;
	
	BOOL _parseLineBreaks;
	BOOL _parseURLs;
	
}

@property (nonatomic, retain) BBStyledNode *rootNode;
@property (nonatomic) BOOL parseLineBreaks;
@property (nonatomic) BOOL parseURLs;

- (void)parseText:(NSString *)string;

@end
