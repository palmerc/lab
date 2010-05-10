//
//  BBStyledNode.h
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

@interface BBStyledNode : NSObject {
	BBStyledNode *_nextSibling;
	BBStyledNode *_parentNode;
}

@property (nonatomic, retain) BBStyledNode *nextSibling;
@property (nonatomic, assign) BBStyledNode *parentNode;
@property (nonatomic, readonly) NSString *outerText;
@property (nonatomic, readonly) NSString *outerHTML;

- (id)initWithNextSibling:(BBStyledNode *)nextSibling;
- (id)ancestorOrSelfWithClass:(Class)c;
- (void)performDefaultAction;

@end
