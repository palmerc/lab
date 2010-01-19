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
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
	
	[chart release];
	chart = nil;
}

-(void) testChartParsingWithImage {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";	
	NSString *request = @"Request: Chart/OK";
	NSString *secOid = @"SecOid: 18177/TEL";
	NSString *width = @"Width: 0"; // informational
	NSString *height = @"Height: 0"; // informational
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
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
	
	[chart release];
	chart = nil;
}

#pragma mark Login Unit Tests

-(void) testLogin {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *contentLength = @"Content-Length: 0";
	NSString *blank = @"";
	NSString *request = @"Request: login/OK";
	NSString *version = @"Version: 1.00.00";
	NSString *dload = @"DLURL:"; 
	NSString *serverIP = @"ServerIP: 1.1.1.1";
	NSString *user = @"User: user";
	NSString *symbols = @"Symbols:";
	NSString *exchanges = @"Exchanges:";
	NSString *newsFeeds = @"NewsFeeds:";
	NSString *quotes = @"Quotes:";

	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, contentLength, blank, request, version, dload, serverIP, user, symbols, exchanges, newsFeeds, quotes, nil];
	[communicator.communicator.lineBuffer  setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	[self logBlockBuffer];
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
		NSLog(@"%d", communicator.state);
	}
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Keep-Alive Unit Tests

-(void) testKeepAlives {
	[self loginStarterUpper];

	NSString *request = @"Request: q";
	NSString *blank = @"";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:request, blank, nil];
	[communicator.communicator.lineBuffer setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	[self logBlockBuffer];
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
		NSLog(@"%d", communicator.state);
	}
	
	STAssertTrue(communicator.state == PROCESSING, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Add Stock Unit Tests

-(void) testAddStocks {
	[self loginStarterUpper];
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";
	NSString *request = @"Request: addSec/OK";
	NSString *quotes = @"SecInfo:";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, quotes, nil];
	[communicator.communicator.lineBuffer setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	[self logBlockBuffer];
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	
	STAssertTrue(communicator.state == PROCESSING, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testAddStocksParsing {
	[self loginStarterUpper];
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	
	NSString *request = @"Request: addSec/OK";
	NSString *secInfo = @"SecInfo: 18177/EMS;EMS;Eitzen Maritime Services;Oslo Stocks [OSS];1;D;NO0003075905;15329";
	NSString *blank = @"";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, secInfo, nil];
	[communicator.communicator.lineBuffer setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	[self logBlockBuffer];
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
}
	
	STAssertTrue(communicator.state == PROCESSING, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Real-Time Quotes Unit Tests

-(void) testRealTimeQuotes {
	[self loginStarterUpper];
	
	NSString *request = @"Request: q";
	NSString *quotes = @"Quotes:";
	NSString *blank = @"";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:request, quotes, blank, nil];
	[communicator.communicator.lineBuffer setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	[self logBlockBuffer];
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
		NSLog(@"%d", communicator.state);
	}
	
	STAssertTrue(communicator.state == PROCESSING, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Quotes Parsing Unit Tests



#pragma mark Helper Methods

-(void) loginStarterUpper {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *contentLength = @"Content-Length: 0";
	NSString *blank = @"";
	NSString *request = @"Request: login/OK";
	NSString *version = @"Version: 1.00.00";
	NSString *dload = @"DLURL:"; 
	NSString *serverIP = @"ServerIP: 1.1.1.1";
	NSString *user = @"User: user";
	NSString *symbols = @"Symbols:";
	NSString *exchanges = @"Exchanges:";
	NSString *newsFeeds = @"NewsFeeds:";
	NSString *quotes = @"Quotes:";
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, contentLength, blank, request, version, dload, serverIP, user, symbols, exchanges, newsFeeds, quotes, blank, nil];
	[communicator.communicator.lineBuffer  setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	[self logBlockBuffer];
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
		
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

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
