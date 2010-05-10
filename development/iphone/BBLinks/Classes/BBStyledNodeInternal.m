//
//  BBStyledNodeInternal.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBStyledNodeInternal.h"

@implementation BBStyledNode (BBInternal)

- (BBStyledNode *)findLastSibling:(BBStyledNode *)sibling {
	while (sibling) {
		if (!sibling.nextSibling) {
			return sibling;
		}
		sibling = sibling.nextSibling;
	}
	return nil;
}

@end
