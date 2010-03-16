//
//  SymbolDataController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@class mTraderCommunicator;
@class Feed;
@class Symbol;
@class NewsFeed;
@class BidAsk;

@interface SymbolDataController : NSObject <NSFetchedResultsControllerDelegate, SymbolsDataDelegate> {
@private
	mTraderCommunicator *communicator;
	
	NSFetchedResultsController *_fetchedResultsController;
	NSManagedObjectContext *_managedObjectContext;
}

@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;

+ (SymbolDataController *)sharedManager;

- (BidAsk *)fetchBidAskForFeedTicker:(NSString *)feedTicker atIndex:(NSUInteger)index;
- (NewsFeed *)fetchNewsFeed:(NSString *)mCode;
- (Feed *)fetchFeed:(NSString *)mCode;
- (Feed *)fetchFeedByName:(NSString *)feedName;
- (void)addSymbols:(NSString *)symbols;
- (void)deleteAllSymbols;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode;
- (void)removeSymbol:(Symbol *)symbol;

@end
