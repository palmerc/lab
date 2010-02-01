//
//  iTraderCommunicatorUnitTests.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#import "iTraderCommunicator.h"

@interface iTraderCommunicatorUnitTests : SenTestCase {
	iTraderCommunicator *communicator;
}

-(void) loginStarterUpper;
-(NSArray *) blockGeneratorWithObjects:(id)object, ... NS_REQUIRES_NIL_TERMINATION;
-(NSData *) stringToLatin1Data:(NSString *)string;
-(NSString *) latin1DataToString:(NSData *)data;
-(void) logBlockBuffer;
@end
