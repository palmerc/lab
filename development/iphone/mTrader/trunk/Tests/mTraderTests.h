//
//  iTraderTests.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#import <UIKit/UIKit.h>
@class mTraderCommunicator;

@interface mTraderTests : SenTestCase {
	iTraderCommunicator *communicator;
}

-(NSArray *) blockGeneratorWithStrings:(id)strings, ...;
-(NSData *) stringToLatin1Data:(NSString *)string;

@end
