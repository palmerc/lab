//
//  BBStyledElement.h
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledNode.h"

@interface BBStyledElement : BBStyledNode {
	BBStyledNode *_firstChild;
	BBStyledNode *_lastChild;
	NSString *_className;
}

@property (nonatomic, readonly) BBStyledNode *firstChild;
@property (nonatomic, readonly) BBStyledNode *lastChild;
@property (nonatomic, retain) NSString *className;

- (id)initWithText:(NSString *)text;
- (id)initWithText:(NSString *)text next:(BBStyledNode *)nextSibling;

- (void)addChild:(BBStyledNode *)child;
- (void)addText:(NSString *)text;
- (void)replaceChild:(BBStyledNode *)oldChild withChild:(BBStyledNode *)newChild;

- (BBStyledNode *)getElementByClassName:(NSString *)className;

@end
