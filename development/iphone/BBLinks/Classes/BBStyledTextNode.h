//
//  BBStyledTextNode.h
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledNode.h"

@interface BBStyledTextNode : BBStyledNode {
	NSString *_text;
}

@property (nonatomic, retain) NSString *text;

- (id)initWithText:(NSString *)text;

- (id)initWithText:(NSString *) text next:(BBStyledNode *)nextSibling;

@end
