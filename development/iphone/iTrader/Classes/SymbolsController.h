//
//  StocksController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "iTraderCommunicator.h"

@protocol SymbolsUpdateDelegate;

@interface SymbolsController : NSObject <SymbolsDataDelegate> {
	id <SymbolsUpdateDelegate> updateDelegate;
	iTraderCommunicator *communicator;
	
	NSMutableDictionary *symbols;
	NSMutableArray *orderedSymbols;
	NSMutableDictionary *feeds;
	NSMutableArray *orderedFeeds;
}

@property (assign) id <SymbolsUpdateDelegate> updateDelegate;
@property (nonatomic, retain) NSMutableDictionary *symbols;
@property (nonatomic, retain) NSMutableArray *orderedSymbols;
@property (nonatomic, retain) NSMutableDictionary *feeds;
@property (nonatomic, retain) NSMutableArray *orderedFeeds;

+ (SymbolsController *)sharedManager;
- (NSArray *)cleanQuote:(NSString *)quote;

@end

@protocol SymbolsUpdateDelegate <NSObject>
-(void)symbolsUpdated:(NSArray *)quotes;
@end


