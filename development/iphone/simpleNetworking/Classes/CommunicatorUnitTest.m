//
//  CommunicatorUnitTest.m
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 18.12.09.
//  Copyright 2009 Bird And Bear Productions. All rights reserved.
//

#import "CommunicatorUnitTest.h"
#import "Communicator.h"


@implementation CommunicatorUnitTest

- (void)setUp {
}

- (void)tearDown {
}

// Test that init leaves the object in its expected state upon completion.
- (void)testInitWithoutArguments {
	Communicator *comm = [[Communicator alloc] init];
	STAssertNil(comm.host, @"Host address");
	STAssertEquals((NSInteger)0, comm.port, @"Port Number");
	
	STAssertNil(comm.inputStream, @"NSInputStream");
	STAssertNil(comm.outputStream, @"NSOutputStream");
	STAssertNil(comm.inputBuffer, @"NSMutableData");
	STAssertNil(comm.outputBuffer, @"NSMutableData");
	
	STAssertEquals([NSNumber numberWithInt:0], comm.bytesRead, @"Bytes Read");
	STAssertEquals(0, comm.byteIndex, @"Byte Index");
	
	STAssertEquals(NO, comm.isConnected, @"isConnected");
	
	[comm release];
}

// Test that init leaves the object in its expected state upon completion.
- (void)testInitWithArguments {
	NSString *url = @"www.google.com";
	NSInteger wwwPort = 80;
	
	Communicator *comm = [[Communicator alloc] initWithSocket:url port:wwwPort];
	STAssertEquals(url, comm.host, @"Host address");
	STAssertEquals(wwwPort, comm.port, @"Port Number");
	
	STAssertNil(comm.inputStream, @"NSInputStream");
	STAssertNil(comm.outputStream, @"NSOutputStream");
	STAssertNil(comm.inputBuffer, @"NSMutableData");
	STAssertNil(comm.outputBuffer, @"NSMutableData");
	
	STAssertEquals([NSNumber numberWithInt:0], comm.bytesRead, @"Bytes Read");
	STAssertEquals(0, comm.byteIndex, @"Byte Index");
	
	STAssertEquals(NO, comm.isConnected, @"isConnected");
	
	[comm release];
}

- (void)testConnectingToWebServer {
	NSString *get = @"GET / HTTP/1.0\r\n\r\n";
	NSString *url = @"www.google.com";
	NSInteger wwwPort = 80;
	NSData *httpGet = [NSData dataWithBytes:[get UTF8String] length:sizeof([get UTF8String])];
	
	Communicator *comm = [[Communicator alloc] initWithSocket:url port:wwwPort];
	
	[comm write:httpGet];	
	
	STAssertEquals(url, comm.host, @"Host address");
	STAssertEquals(wwwPort, comm.port, @"Port Number");
	
	STAssertNotNil(comm.inputStream, @"NSInputStream");
	STAssertNotNil(comm.outputStream, @"NSOutputStream");
	STAssertNil(comm.inputBuffer, @"NSMutableData");
	STAssertNotNil(comm.outputBuffer, @"NSMutableData");
	
	STAssertEquals(YES, comm.isConnected, @"isConnected");
	
	[comm release];	
}

@end
