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

@interface mTraderServerMonitor : NSObject <mTraderServerMonitorDelegate, UIAlertViewDelegate, CommunicatorStatusDelegate> {
@private
	Reachability *_reachability;

	
	NSString *_server;
	NSString *_port;
	
	BOOL _connected;
	BOOL _loggedIn;
}
@property (nonatomic, retain) Reachability *reachability;
@property (nonatomic, retain) NSString *server;
@property (nonatomic, retain) NSString *port;
@property (readonly) BOOL connected;
@property (readonly) BOOL loggedIn;

+ (mTraderServerMonitor *)sharedManager;
- (void)attemptConnection;
- (void)logout;
- (void)disconnect;
- (void)startReachability;

@end
