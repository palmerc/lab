//
//  iTraderCommunicatorUnitTests.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "iTraderCommunicatorUnitTests.h"


@implementation iTraderCommunicatorUnitTests

#pragma mark Setup and Teardown

-(void) setUp {
	communicator = [iTraderCommunicator sharedManager];
	communicator.blockBuffer = [[NSMutableArray alloc] init];
}

-(void) tearDown {
	[communicator.blockBuffer release];
	communicator.blockBuffer = nil;
}

#pragma mark Chart Unit Tests

-(void) testChartParsing {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";	
	NSString *request = @"Request: Chart/OK";
	NSString *secOid = @"SecOid: 18177/TEL";
	NSString *width = @"Width: 0";
	NSString *height = @"Height: 0";
	NSString *imgType = @"ImgType: PNG";
	NSString *imageSize = @"ImageSize: 0";
	NSString *imageData = @"<ImageBegin><ImageEnd>";
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, secOid, width, height, imgType, imageSize, imageData, nil]; 
	
	communicator.blockBuffer = [[NSMutableArray alloc] init];
	
	// This sets an empty image message into the block buffer
	[communicator.blockBuffer setArray:block];
	STAssertTrue([communicator.blockBuffer count] == [block count], @"The block buffer was not filled.");
	[self logBlockBuffer];
	
	// Parse the empty image and make sure it clears out the block correctly
	Chart *chart = [communicator chartParsing];
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared.");
	
	[chart release];
	chart = nil;
}

-(void) testChartParsingWithImage {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";	
	NSString *request = @"Request: Chart/OK";
	NSString *secOid = @"SecOid: 18177/TEL";
	NSString *width = @"Width: 0";
	NSString *height = @"Height: 0";
	NSString *imgType = @"ImgType: PNG";
	
	NSString *filePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"infront" ofType:@"png"];
	STAssertNotNil(filePath, @"File path for test image is nil");
	NSString *imageBegin = @"<ImageBegin>";
	NSString *imageEnd = @"<ImageEnd>";
	NSData *imageData = [NSData dataWithContentsOfFile:filePath];
	NSMutableData *image = [NSMutableData dataWithData:[self stringToLatin1Data:imageBegin]];
	[image appendData:imageData];
	[image appendData:[self stringToLatin1Data:imageEnd]];
	
	NSString *imageSize = [NSString stringWithFormat:@"ImageSize: %d", [imageData length]];
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, secOid, width, height, imgType, imageSize, image, nil]; 
	
	
	// This sets an empty image message into the block buffer
	[communicator.blockBuffer setArray:block];
	STAssertTrue([communicator.blockBuffer count] == [block count], @"The block buffer was not filled.");
	[self logBlockBuffer];
	
	// Parse the empty image and make sure it clears out the block correctly
	Chart *chart = [communicator chartParsing];
	STAssertNotNil(chart, @"Chart was nil");
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared.");
	
	[chart release];
	chart = nil;
}


#pragma mark Helper Methods

-(NSArray *) blockGeneratorWithObjects:(id)firstArg, ... {
	NSMutableArray *block = [[NSMutableArray alloc] init];
	
	NSMutableArray *argArray = [[NSMutableArray alloc] init]; // Array is just for capturing our argument list
	va_list argList;
	va_start(argList, firstArg);
	for (id arg = firstArg; arg != nil; arg = va_arg(argList, id)) {
		[argArray addObject:arg];
	}
	va_end(argList);
	
	NSData *carriageReturnLineFeed = [self stringToLatin1Data:@"\r\n"];
	for (id arg in argArray) {
		NSMutableData *dataLine = [[NSMutableData alloc] init];
		if ([arg isKindOfClass:[NSString class]]) {
			[dataLine appendData:[self stringToLatin1Data:arg]];
			[dataLine appendData:carriageReturnLineFeed];
			[block addObject:dataLine];
		} else if ([arg isKindOfClass:[NSData class]]) {
			[dataLine appendData:arg];
			[dataLine appendData:carriageReturnLineFeed];
			[block addObject:dataLine];
		} else {
			NSLog(@"blockGeneratorWithObjects: Object added was neither an NSString or NSData object. It was %@", arg);
		}
		[dataLine release];
		dataLine = nil;
	}
	[argArray release];
	
	//[block addObject:carriageReturnLineFeed];
	
	NSArray *finalProduct = [NSArray arrayWithArray:block];
	[block release];
	return finalProduct;
}

-(NSData *) stringToLatin1Data:(NSString *)string {
	return [string dataUsingEncoding:NSISOLatin1StringEncoding];
}

-(NSString *) latin1DataToString:(NSData *)data {
	return [[[NSString alloc] initWithData:data encoding:NSISOLatin1StringEncoding] autorelease];
}

-(void) logBlockBuffer {
	for (NSData *data in communicator.blockBuffer) {
		 NSLog(@">>>%@<<<", [self latin1DataToString:data]);
	}

}

@end
