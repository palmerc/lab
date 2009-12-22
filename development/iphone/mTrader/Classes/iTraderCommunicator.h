//
//  mTraderCommunicator.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Communicator.h";

@interface iTraderCommunicator : NSObject <CommunicatorReceiveDelegate> {
	Communicator *_communicator;
	BOOL _isLoggedIn;
	BOOL _loginStatusHasChanged;
}

@property (nonatomic, retain) Communicator *communicator;
@property BOOL isLoggedIn;

- (void)login:(NSString *)username password:(NSString *)password;
- (BOOL)loginStatusHasChanged;

@end
