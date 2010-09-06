//
//  NSData+StringAdditions.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NSData+StringAdditions.h"


@implementation NSData (StringAdditions)

- (NSString *)string {
	return [[[NSString alloc] initWithData:self encoding:NSISOLatin1StringEncoding] autorelease];
}

@end
