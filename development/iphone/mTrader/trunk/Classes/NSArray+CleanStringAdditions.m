//
//  NSArray+CleanStringAdditions.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NSArray+CleanStringAdditions.h"

#import "NSString+CleanStringAdditions.h"

@implementation NSArray (CleanStringAdditions)

- (NSArray *)sansWhitespace {
	NSMutableArray *cleanedStrings = [NSMutableArray arrayWithCapacity:[self count]];
	for (NSString *string in self) {
		[cleanedStrings addObject:[string sansWhitespace]];
	}
	
	return cleanedStrings;
}

@end
