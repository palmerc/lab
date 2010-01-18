//
//  iTraderTests.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "iTraderTests.h"


@implementation iTraderTests

-(void) testAppDelegate {
	id app_delegate = [[UIApplication sharedApplication] delegate];
	STAssertNotNil(app_delegate, @"Cannot find the application delegate.");
}

@end
