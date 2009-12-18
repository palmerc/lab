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
}

@end
