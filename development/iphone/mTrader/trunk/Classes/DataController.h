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
@class SymbolNewsRelationship;
@class BidAsk;

@protocol OrderBookDelegate;
@protocol TradesDelegate;
@protocol SearchResultsDelegate;

@interface DataController : NSObject <NSFetchedResultsControllerDelegate, SymbolsDataDelegate> {
@private
	id <OrderBookDelegate> orderBookDelegate;
	
	NSFetchedResultsController *_fetchedResultsController;
	NSManagedObjectContext *_managedObjectContext;
	
	NSUInteger symbolIndex;
	
	NSDateFormatter *_dateFormatter;
	NSDateFormatter *_dateTimeFormatter;
	NSDateFormatter *_yearFormatter;
}

@property (assign) id <OrderBookDelegate> orderBookDelegate;
@property (assign) id <TradesDelegate> tradesDelegate;
@property (assign) id <SearchResultsDelegate> searchDelegate;

@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;

+ (DataController *)sharedManager;

- (BidAsk *)fetchBidAskForFeedTicker:(NSString *)feedTicker atIndex:(NSUInteger)index;
- (NSArray *)fetchBidAsksForSymbol:(NSString *)tickerSymbol withFeedNumber:(NSString *)feedNumber;
- (Trade *)fetchTradeForSymbol:(NSString *)feedTicker atIndex:(NSUInteger)index;

- (NewsFeed *)fetchNewsFeed:(NSString *)mCode;
- (NewsFeed *)fetchNewsFeedWithNumber:(NSString *)feedNumber;
- (SymbolNewsRelationship *)fetchRelationshipForArticle:(NewsArticle *)article andSymbol:(Symbol *)symbol;

- (NewsArticle *)fetchNewsArticle:(NSString *)articleNumber withFeed:(NSString *)feedNumber;

- (Feed *)fetchFeed:(NSString *)mCode;
- (Feed *)fetchFeedByName:(NSString *)feedName;
- (Feed *)fetchFeedByNumber:(NSString *)feedNumber;

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSString *)feedNumber;
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode;
- (NSArray *)fetchAllSymbols;

- (Chart *)fetchChart:(NSString *)tickerSymbol withFeedNumber:(NSString *)feedNumber;

- (void)addSymbols:(NSString *)symbols;
- (void)removeSymbol:(Symbol *)symbol;

- (void)deleteAllNews;
- (void)deleteAllBidsAsks;
- (void)deleteTradesForFeedTicker:(NSString *)feedTicker;

- (void)maxNewsArticles:(NSInteger)max;

- (NSArray *)splitFeedTicker:(NSString *)feedTicker;

@end

@protocol TradesDelegate <NSObject>
- (void)updateTrades;
@end

@protocol SearchResultsDelegate <NSObject>
- (void)searchResultsUpdate:(NSArray *)results;
@end