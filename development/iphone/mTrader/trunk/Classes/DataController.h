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
@class Trade;
@class NewsFeed;
@class NewsArticle;
@class BidAsk;

@protocol OrderBookDelegate;
@protocol TradesDelegate;
@protocol SearchResultsDelegate;

@interface DataController : NSObject <NSFetchedResultsControllerDelegate, SymbolsDataDelegate> {
@private
	id <OrderBookDelegate> orderBookDelegate;
	id <TradesDelegate> tradesDelegate;
	id <SearchResultsDelegate> searchDelegate;
	
	NSFetchedResultsController *_fetchedResultsController;
	NSManagedObjectContext *_managedObjectContext;
}

@property (nonatomic, assign) id <OrderBookDelegate> orderBookDelegate;
@property (nonatomic, assign) id <TradesDelegate> tradesDelegate;
@property (nonatomic, assign) id <SearchResultsDelegate> searchDelegate;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;

+ (DataController *)sharedManager;
+ (NSArray *)fetchBidAsksForSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber inManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
+ (NSArray *)fetchTradesForSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber inManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

- (BidAsk *)fetchBidAskForFeedTicker:(NSString *)feedTicker atIndex:(NSUInteger)index;
- (Trade *)fetchTradeForSymbol:(NSString *)feedTicker atIndex:(NSUInteger)index;
- (NewsFeed *)fetchNewsFeed:(NSString *)mCode;
- (NewsFeed *)fetchNewsFeedWithNumber:(NSString *)feedNumber;
- (NewsArticle *)fetchNewsArticle:(NSString *)articleNumber withFeed:(NSString *)feedNumber;

- (Feed *)fetchFeed:(NSString *)mCode;
- (Feed *)fetchFeedByName:(NSString *)feedName;
- (Feed *)fetchFeedByNumber:(NSString *)feedNumber;
- (void)addSymbols:(NSString *)symbols;
- (void)maxNewsArticles:(NSInteger)max;
- (void)deleteAllNews;
- (void)deleteAllSymbols;
- (void)deleteAllBidsAsks;
- (void)deleteTradesForFeedTicker:(NSString *)feedTicker;
- (NSArray *)fetchAllSymbols;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode;
- (Chart *)fetchChart:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;
- (void)removeSymbol:(Symbol *)symbol;

@end

@protocol OrderBookDelegate <NSObject>
- (void)updateOrderBook;
@end

@protocol TradesDelegate <NSObject>
- (void)updateTrades;
@end

@protocol SearchResultsDelegate <NSObject>
- (void)searchResultsUpdate:(NSArray *)results;
@end