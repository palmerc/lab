//
//  NSArray+StripFirstElementAdditions.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NSArray+StripFirstElementAdditions.h"


@implementation NSArray (StripFirstElementAdditions)

- (NSArray *)stripFirstElement {
	NSRange rowsWithoutFirstString;
	rowsWithoutFirstString.location = 1;
	rowsWithoutFirstString.length = [self count] - 1;
	return [self subarrayWithRange:rowsWithoutFirstString];
}

@end
