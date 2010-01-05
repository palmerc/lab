//
//  StocksController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "iTraderCommunicator.h"

@interface SymbolsController : NSObject <SymbolsDataDelegate> {
	iTraderCommunicator *communicator;
	
	NSMutableArray *symbols;
	NSMutableDictionary *feeds;
}

@property (nonatomic, retain) NSMutableArray *symbols;
@property (nonatomic, retain) NSMutableDictionary *feeds;

+ (SymbolsController *)sharedManager;

@end
