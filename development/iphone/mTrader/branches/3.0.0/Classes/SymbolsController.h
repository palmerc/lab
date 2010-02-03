//
//  StocksController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "mTraderCommunicator.h"
@protocol SymbolsUpdateDelegate;

@interface SymbolsController : NSObject <mTraderServerDataDelegate> {
	id <SymbolsUpdateDelegate> updateDelegate;
	mTraderCommunicator *_communicator;
	
	NSMutableArray *_feeds;
	NSMutableArray *_exchanges;
}

@property (assign) id <SymbolsUpdateDelegate> updateDelegate;
@property (nonatomic, assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSMutableArray *feeds;
@property (nonatomic, retain) NSMutableArray *exchanges;

// Singleton Class Method
+ (SymbolsController *)sharedManager;

- (NSArray *)cleanQuote:(NSString *)quote;
- (void)addSymbol:(Symbol *)symbol;
- (void)addFeed:(Feed *)feed;
-(void) removeSymbol:(NSIndexPath *)indexPath;
- (NSInteger)indexOfFeed:(Feed *)feed;
- (NSInteger)indexOfFeedWithFeedNumber:(NSString *)feedNumber;
- (NSIndexPath *)indexPathOfSymbol:(NSString *)feedTicker;
- (Symbol *)symbolAtIndexPath:(NSIndexPath *)indexPath;
- (Symbol *)symbolWithFeedTicker:(NSString *)feedTicker;
@end

@protocol SymbolsUpdateDelegate <NSObject>
@optional
- (void)symbolsAdded:(NSArray *)symbols;
-(void) symbolRemoved:(NSIndexPath *)indexPath;
- (void)feedAdded:(Feed *)feed;
- (void)symbolsUpdated:(NSArray *)quotes;
- (void)staticUpdated:(NSString *)feedTicker;
@end


