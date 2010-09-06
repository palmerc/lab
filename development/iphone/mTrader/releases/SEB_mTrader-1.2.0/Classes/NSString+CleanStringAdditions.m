//
//  NSString+CleanStringAdditions.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NSString+CleanStringAdditions.h"


@implementation NSString (CleanStringAdditions)

- (NSString *)sansWhitespace {
	NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
	return [self stringByTrimmingCharactersInSet:whitespaceAndNewline];
}

@end
