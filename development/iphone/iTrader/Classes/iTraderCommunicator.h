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
@property (readonly) BOOL isLoggedIn;

+ (iTraderCommunicator *)sharedManager;

- (void)login;
- (void)logout;
- (BOOL)loginStatusHasChanged;
- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings;
@end

@protocol iTraderCommunicatorStatusDelegate <NSObject>
- (void)connectionStatusHasChanged:(BOOL)isConnected;
@end

@protocol iTraderCommunicatorUpdateDelegate <NSObject>
- (void)streamingUpdateReceived;
@end