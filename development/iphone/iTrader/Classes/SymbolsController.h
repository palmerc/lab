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

@interface SymbolsController : NSObject <mTraderServerDataDelegate> {
	id <SymbolsUpdateDelegate> updateDelegate;
	iTraderCommunicator *_communicator;
	
	NSMutableArray *_feeds;
}

@property (assign) id <SymbolsUpdateDelegate> updateDelegate;
@property (nonatomic, retain) NSMutableArray *feeds;

+ (SymbolsController *)sharedManager;

- (NSArray *)cleanQuote:(NSString *)quote;
- (void)addSymbol:(Symbol *)symbol;
- (void)addFeed:(Feed *)feed;
- (NSInteger)indexOfFeed:(Feed *)feed;
- (NSInteger)indexOfFeedWithFeedNumber:(NSString *)feedNumber;
- (NSIndexPath *)indexPathOfSymbol:(NSString *)feedTicker;
- (Symbol *)symbolAtIndexPath:(NSIndexPath *)indexPath;
- (Symbol *)symbolWithFeedTicker:(NSString *)feedTicker;

@end

@protocol SymbolsUpdateDelegate <NSObject>
- (void)symbolsAdded:(NSArray *)symbols;
- (void)feedAdded:(Feed *)feed;
- (void)symbolsUpdated:(NSArray *)quotes;
- (void)staticUpdated:(NSString *)feedTicker;
@end


