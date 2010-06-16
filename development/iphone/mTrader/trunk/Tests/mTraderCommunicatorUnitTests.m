//
//  mTraderCommunicatorUnitTests.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderCommunicatorUnitTests.h"

#import "mTraderCommunicator.h"

@implementation mTraderCommunicatorUnitTests

#pragma mark Setup and Teardown

-(void) setUp {
	communicator = [mTraderCommunicator sharedManager];
}

-(void) tearDown {
	communicator.state = HEADER;
}

#pragma mark Header Parsing Unit Tests

-(void) testHeaderParsing {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	
	STAssertTrue(communicator.state == STATICRESPONSE, @"The state should be static-response. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testHeaderParsingWithContent {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *content = @"Content-Length: 0";
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, content, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	
	STAssertTrue(communicator.state == FIXEDLENGTH, @"The state should be fixed-length. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Chart Unit Tests

-(void) testChartParsing {
	[self loginStarterUpper];
	
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
	
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	// Parse the empty image and make sure it clears out the block correctly
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testChartParsingWithImage {
	[self loginStarterUpper];
	
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
	
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	// Parse the empty image and make sure it clears out the block correctly
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testCompleteChartParsingWithImage {
	[self loginStarterUpper];
	
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
	
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	// Parse the empty image and make sure it clears out the block correctly
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Login Unit Tests

-(void) testLogin {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
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

	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, version, dload, serverIP, user, symbols, exchanges, newsFeeds, quotes, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.contentLength == 0, @"The content length should be zero instead it was %d", communicator.contentLength);
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testLoginWithNoSymbols {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";
	NSString *request = @"Request: login/OK";
	NSString *version = @"Version: 1.00.00";
	NSString *dload = @"DLURL:"; 
	NSString *serverIP = @"ServerIP: 1.1.1.1";
	NSString *user = @"User: user";
	NSString *symbols = @"Symbols: ";
	NSString *exchanges = @"Exchanges: ";
	NSString *newsFeeds = @"NewsFeeds: ";
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, version, dload, serverIP, user, symbols, exchanges, newsFeeds, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.contentLength == 0, @"The content length should be zero instead it was %d", communicator.contentLength);
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testLoginEpicFailUserPassword {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *contentLength = @"Content-Length: 32";
	NSString *blank = @"";
	NSString *request = @"Request: login/failed.UsrPwd";
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, contentLength, blank, request, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.contentLength == 0, @"The content length should be zero instead it was %d", communicator.contentLength);
	STAssertTrue(communicator.state == LOGIN, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testLoginEpicFailDeniedAccess {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *contentLength = @"Content-Length: 38";
	NSString *blank = @"";
	NSString *request = @"Request: login/failed.DeniedAccess";
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, contentLength, blank, request, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.contentLength == 0, @"The content length should be zero instead it was %d", communicator.contentLength);
	STAssertTrue(communicator.state == LOGIN, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Keep-Alive Unit Tests

-(void) testKeepAlives {
	[self loginStarterUpper];

	NSString *request = @"Request: q";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:request, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	
	STAssertTrue(communicator.state == PROCESSING, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Add/Remove Stock Unit Tests

-(void) testAddStocks {
	[self loginStarterUpper];
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";
	NSString *request = @"Request: addSec/OK";
	NSString *quotes = @"SecInfo:";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, quotes, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
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
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	
	STAssertTrue(communicator.state == PROCESSING, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testRemoveStocksParsing {
	[self loginStarterUpper];
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *request = @"Request: remSec/OK";
	NSString *secInfo = @"SecInfo:";
	NSString *blank = @"";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, secInfo, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
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
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:request, quotes, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	
	STAssertTrue(communicator.state == PROCESSING, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Kickout Unit Tests

-(void) testKickOut {
	[self loginStarterUpper];
	
	NSString *request = @"Request: q";
	NSString *kickout = @"Kickout: 1";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:request, kickout, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	
	STAssertTrue(communicator.state == KICKOUT, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

#pragma mark Static Data Unit Tests

-(void) testStaticData {
	[self loginStarterUpper];
	
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";
	NSString *request = @"Request: StaticData/OK";
	NSString *secOid = @"SecOid:";
	NSString *staticData = @"StaticData:";
	
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"The line buffer was not zero.");
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, secOid, staticData, nil];
	[communicator.communicator.lineBuffer addObjectsFromArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	
	STAssertTrue(communicator.state == PROCESSING, @"This is a keep-alive and should cause state to revert to Processing. State is %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}
/*
#pragma mark News Feeds Unit Tests

-(void) testNewsResponse {
	[self loginStarterUpper];
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";
	NSString *request = @"Request: NewsBody/OK";
	NSString *newsId = @"NewsID: ";
	NSString *time = @"Time:";
	NSString *flags = @"Flags:";
	NSString *headline = @"Headline:";
	NSString *body = @"Body:";
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, newsId, time, flags, headline, body, nil];
	
	[communicator.communicator.lineBuffer  setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.contentLength == 0, @"The content length should be zero instead it was %d", communicator.contentLength);
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}

-(void) testNewsListsFeeds {
	[self loginStarterUpper];
	
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
	NSString *blank = @"";
	NSString *request = @"Request: NewsListFeeds/OK";
	NSString *newsFeeds = @"NewsFeeds: AllNews";
	// The news line is separated by '|' and 5 semi-colon separated fields.
	NSString *news = @"News: 18182/19;;22.01;10:03;NOK Bond loan  FIBO1 Pro|17674/2842;;22.01;09:57;NeuroSearch/CEO: FDA giver positivt respons p. fase 3 protokol|1073/01221279;;22.01;09:56;DJ Handyhersteller Sony Ericsson d.mmt Verlust im unerwartet 4Q ein|17668/457832;;22.01;09:56;Formuepleje Pareto A/S Indrev.rdi Formuepleje Pareto A/S indre v.rdi: 236,7 pr. aktie.|17930/1f61d08805;;22.01;09:56;Westander : Westanders vinst .kar|17674/2841;F;22.01;09:55;***COLOPLAST/ALM BRAND: 'NEUTRAL' - KURSM.L 490 KR";
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, newsFeeds, news, nil];
	[communicator.communicator.lineBuffer  setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.contentLength == 0, @"The content length should be zero instead it was %d", communicator.contentLength);
	STAssertTrue(communicator.state == PROCESSING, @"The state should be processing after login. State was %d", communicator.state);
	STAssertTrue([communicator.blockBuffer count] == 0, @"The block buffer was not cleared out correctly");
	STAssertTrue([communicator.communicator.lineBuffer count] == 0, @"Line buffer not emptied.");
}
*/
#pragma mark Helper Methods

-(void) loginStarterUpper {
	NSString *httpHeader = @"HTTP/1.1 200 OK";
	NSString *server = @"Server: MMS";
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
	
	NSArray *block = [self blockGeneratorWithObjects:httpHeader, server, blank, request, version, dload, serverIP, user, symbols, exchanges, newsFeeds, quotes, nil];
	[communicator.communicator.lineBuffer  setArray:block];
	STAssertTrue([communicator.communicator.lineBuffer count] == [block count], @"The block buffer was not filled.");
	
	for (int i=0; i < [block count]; i++) {
		[communicator dataReceived];
	}
	STAssertTrue(communicator.contentLength == 0, @"The content length should be zero instead it was %d", communicator.contentLength);
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
	
	[block addObject:[self stringToLatin1Data:@"\r\r"]];
	
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
