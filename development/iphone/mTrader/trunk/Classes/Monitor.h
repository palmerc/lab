//
//  mTraderServerMonitor.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderCommunicator.h"
#import "Communicator.h"
#import "Reachability.h"

@class Communicator;
@class StatusController;

@interface Monitor : NSObject <mTraderStatusDelegate, UIAlertViewDelegate, CommunicatorStatusDelegate> {
@private
	Communicator *_communicator;
	mTraderCommunicator *_mTCom;
	
	Reachability *_reachability;
	
	NSURL *_url;
	NSString *_host;
	NSUInteger _port;
	
	BOOL _connecting;
	BOOL _connected;
	BOOL _loggedIn;
	
	StatusController *_statusController;
}

@property (readonly) NSString *host;
@property (readonly) NSUInteger port;
@property (readonly) BOOL connected;
@property (readonly) BOOL loggedIn;
@property (readonly) NSUInteger bytesReceived;
@property (readonly) NSUInteger bytesSent;
@property (nonatomic, retain) StatusController *statusController;

+ (Monitor *)sharedManager;
- (NetworkStatus)currentReachabilityStatus;
- (void)applicationDidFinishLaunching;
- (void)applicationWillTerminate;
- (void)applicationWillResignActive;
- (void)applicationDidBecomeActive;
- (void)usernameAndPasswordChanged;

- (NSArray *)interfaces;
- (NSArray *)serverAddresses;

@end
