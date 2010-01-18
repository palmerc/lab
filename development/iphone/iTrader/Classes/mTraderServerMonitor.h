//
//  mTraderServerMonitor.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 18.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>
@class Reachability;

@interface mTraderServerMonitor : NSObject {
	Reachability *_reachability;
}
@property (nonatomic,retain) Reachability *reachability;

@end
