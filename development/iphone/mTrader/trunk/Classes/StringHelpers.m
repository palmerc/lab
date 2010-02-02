//
//  StringHelpers.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 29.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "StringHelpers.h"


@implementation StringHelpers

+ (NSArray *)cleanComponents:(NSArray *)arrayOfStrings {
	
	if (arrayOfStrings == nil) {
		return nil;
	}
	
	NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
	NSMutableArray *cleanedStrings = [[NSMutableArray alloc] init];
	for (NSString *string in arrayOfStrings) {
		string = [string stringByTrimmingCharactersInSet:whitespaceAndNewline];
		[cleanedStrings addObject:string];
	}
	
	return cleanedStrings;
}

@end
