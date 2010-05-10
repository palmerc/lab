//
//  BBStyledLinkNode.h
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledInline.h"

@class BBStyledNode;

@interface BBStyledLinkNode : BBStyledInline {
	NSString *_URL;
	BOOL _highlighted;
}

@property (nonatomic) BOOL highlighted;
@property (nonatomic, retain) NSString *URL;

- (id)initWithURL:(NSString *)URL;
- (id)initWithURL:(NSString *)URL next:(BBStyledNode *)nextSibling;
- (id)initWithText:(NSString *)text URL:(NSString *)URL next:(BBStyledNode *)nextSibling;

@end
