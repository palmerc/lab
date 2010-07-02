//
//  NSArray+ArrayToDataStringAdditions.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NSArray+ArrayToDataStringAdditions.h"


@implementation NSArray (ArrayToDataStringAdditions)

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
	
	return data;
}

@end
