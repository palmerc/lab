//
//  mTraderTests.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderTests.h"


@implementation mTraderTests

-(void) testAppDelegate {
	id app_delegate = [[UIApplication sharedApplication] delegate];
	STAssertNotNil(app_delegate, @"Cannot find the application delegate.");
}
/*
-(void) testChartParsing {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";	
	NSString *request = @"Request: Chart/OK";
	NSString *secOid = @"SecOid: feedTicker";
	NSString *width = @"Width: 0";
	NSString *height = @"Height: x";
	NSString *imgType = @"ImgType: PNG";
	NSString *imageSize = @"ImageSize: 0";
	NSString *imageData = @"<ImageBegin><ImageEnd>";
	
	NSArray *block = [self blockGeneratorWithStrings:httpHeader, server, blank, request, secOid, width, height, imgType, imageSize, imageData, nil]; 
	
	[communicator.blockBuffer addObjectsFromArray:block];
	STAssertTrue(TRUE, @"");
}
*/

-(NSArray *) blockGeneratorWithStrings:(id)strings, ... {
	NSMutableArray *block = [[NSMutableArray alloc] init];
	
	NSData *carriageReturnLineFeed = [self stringToLatin1Data:@"\r\n"];
	for (NSString *string in strings) {
		[block addObject:[self stringToLatin1Data:string]];
		[block addObject:carriageReturnLineFeed];
	}
	
	[block addObject:carriageReturnLineFeed];
	return block;
}

-(NSData *) stringToLatin1Data:(NSString *)string {
	return [string dataUsingEncoding:NSISOLatin1StringEncoding];
}


@end
