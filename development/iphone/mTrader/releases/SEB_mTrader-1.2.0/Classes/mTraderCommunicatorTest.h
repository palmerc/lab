//
//  mTraderCommunicatorTest.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.06.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define USE_APPLICATION_UNIT_TEST 1

#import <SenTestingKit/SenTestingKit.h>
#import <UIKit/UIKit.h>

@class mTraderCommunicator;
@interface mTraderCommunicatorTest : SenTestCase {
	mTraderCommunicator *communicator;
}

-(void) loginStarterUpper;
-(NSArray *) blockGeneratorWithObjects:(id)object, ... NS_REQUIRES_NIL_TERMINATION;
-(NSData *) stringToLatin1Data:(NSString *)string;
-(NSString *) latin1DataToString:(NSData *)data;
-(void) logBlockBuffer;
@end
