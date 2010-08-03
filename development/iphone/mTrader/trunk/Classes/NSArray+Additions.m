//
//  NSArray+Additions.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NSArray+Additions.h"

#import "NSString+CleanStringAdditions.h"

@implementation NSArray (Additions)

- (NSArray *)sansWhitespace {
	NSMutableArray *cleanedStrings = [NSMutableArray arrayWithCapacity:[self count]];
	for (NSString *string in self) {
		[cleanedStrings addObject:[string sansWhitespace]];
	}
	
	return [[cleanedStrings retain] autorelease];
}

- (NSArray *)stripFirstElement {
	NSRange rowsWithoutFirstString;
	rowsWithoutFirstString.location = 1;
	rowsWithoutFirstString.length = [self count] - 1;
	return [[[self subarrayWithRange:rowsWithoutFirstString] retain] autorelease];
}

- (NSData *)data {
	NSString *EOL = @"\r\n";
	
	NSMutableString *appendableText = [[NSMutableString alloc] init];
	for (NSString *string in self) {
		NSString *current = [[NSString alloc] initWithFormat:@"%@%@", string, EOL];
		[appendableText appendString:current];
		[current release];
	}
	
	[appendableText appendString:EOL]; // A blank line indicates the end of the sending block
	
	NSData *data = [appendableText dataUsingEncoding:NSISOLatin1StringEncoding];
	
	[appendableText release];
	
	return [[data retain] autorelease];
}

@end
