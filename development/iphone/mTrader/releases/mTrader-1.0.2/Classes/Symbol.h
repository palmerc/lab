//
//  Symbol.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 25.06.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class BidAsk;
@class Chart;
@class Feed;
@class NewsArticle;
@class SymbolDynamicData;
@class Trade;

@interface Symbol :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * index;
@property (nonatomic, retain) NSString * tickerSymbol;
@property (nonatomic, retain) NSString * isin;
@property (nonatomic, retain) NSString * type;
@property (nonatomic, retain) NSString * companyName;
@property (nonatomic, retain) NSString * country;
@property (nonatomic, retain) NSString * currency;
@property (nonatomic, retain) NSString * orderBook;
@property (nonatomic, retain) Chart * chart;
@property (nonatomic, retain) Feed * feed;
@property (nonatomic, retain) NSSet* bidsAsks;
@property (nonatomic, retain) NSSet* trades;
@property (nonatomic, retain) SymbolDynamicData * symbolDynamicData;
@property (nonatomic, retain) NSSet* newsArticles;

@end


@interface Symbol (CoreDataGeneratedAccessors)
- (void)addBidsAsksObject:(BidAsk *)value;
- (void)removeBidsAsksObject:(BidAsk *)value;
- (void)addBidsAsks:(NSSet *)value;
- (void)removeBidsAsks:(NSSet *)value;

- (void)addTradesObject:(Trade *)value;
- (void)removeTradesObject:(Trade *)value;
- (void)addTrades:(NSSet *)value;
- (void)removeTrades:(NSSet *)value;

- (void)addNewsArticlesObject:(NewsArticle *)value;
- (void)removeNewsArticlesObject:(NewsArticle *)value;
- (void)addNewsArticles:(NSSet *)value;
- (void)removeNewsArticles:(NSSet *)value;

@end

