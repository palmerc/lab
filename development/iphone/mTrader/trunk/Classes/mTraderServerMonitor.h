//
//  mTraderServerMonitor.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderCommunicator.h"

@class Reachability;

@interface mTraderServerMonitor : NSObject <mTraderServerMonitorDelegate> {
@private
	Reachability *_reachability;
	NSString *_server;
	NSString *_port;
}
@property (nonatomic,retain) Reachability *reachability;
@property (nonatomic,retain) NSString *server;
@property (nonatomic,retain) NSString *port;

+ (mTraderServerMonitor *)sharedManager;

@end
