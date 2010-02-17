//
//  StringHelpers.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 29.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
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
	
	return [cleanedStrings autorelease];
}

+ (NSArray *)componentsOfFeedNameAndCode:(NSString *)feedNameAndCode {
	// Separate the Description from the mCode
	NSRange leftBracketRange = [feedNameAndCode rangeOfString:@"["];
	NSRange rightBracketRange = [feedNameAndCode rangeOfString:@"]"];
	
	NSRange mCodeRange;
	mCodeRange.location = leftBracketRange.location + 1;
	mCodeRange.length = rightBracketRange.location - mCodeRange.location;
	NSString *mCode = [feedNameAndCode substringWithRange:mCodeRange]; // OSS
	
	NSRange descriptionRange;
	descriptionRange.location = 0;
	descriptionRange.length = leftBracketRange.location - 1;
	NSString *feedName = [feedNameAndCode substringWithRange:descriptionRange]; // Oslo Stocks
	
	return [NSArray arrayWithObjects:feedName, mCode, nil];
}
@end
