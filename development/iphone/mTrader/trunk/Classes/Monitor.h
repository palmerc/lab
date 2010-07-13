//
//  mTraderServerMonitor.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderCommunicator.h"
#import "Communicator.h"

@class Reachability;
@class Communicator;

@interface Monitor : NSObject <mTraderStatusDelegate, UIAlertViewDelegate, CommunicatorStatusDelegate> {
@private
	Communicator *_communicator;
	mTraderCommunicator *_mTCom;
	
	Reachability *_reachability;
	
	NSURL *_url;
	NSUInteger _port;
	
	BOOL _connected;
	BOOL _loggedIn;
	
	UIAlertView *_statusAlertView;
}

@property (readonly) BOOL connected;
@property (readonly) BOOL loggedIn;
@property (readonly) NSUInteger bytesReceived;
@property (readonly) NSUInteger bytesSent;

+ (Monitor *)sharedManager;
- (void)applicationDidFinishLaunching;
- (void)applicationWillTerminate;
- (void)applicationWillResignActive;
- (void)applicationDidBecomeActive;
- (void)usernameAndPasswordChanged;
@end
