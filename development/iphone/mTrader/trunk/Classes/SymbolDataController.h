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
@class NewsArticle;
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
+ (NSArray *)fetchBidAsksForSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode inManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

- (BidAsk *)fetchBidAskForFeedTicker:(NSString *)feedTicker atIndex:(NSUInteger)index;
- (NewsFeed *)fetchNewsFeed:(NSString *)mCode;
- (NewsFeed *)fetchNewsFeedWithNumber:(NSString *)feedNumber;
- (NewsArticle *)fetchNewsArticle:(NSString *)articleNumber withFeed:(NSString *)feedNumber;

- (Feed *)fetchFeed:(NSString *)mCode;
- (Feed *)fetchFeedByName:(NSString *)feedName;
- (void)addSymbols:(NSString *)symbols;
- (void)deleteAllNews;
- (void)deleteAllSymbols;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode;
- (void)removeSymbol:(Symbol *)symbol;

@end
