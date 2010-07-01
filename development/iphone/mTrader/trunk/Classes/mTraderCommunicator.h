//
//  mTraderCommunicator.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "mTraderLinesToBlocks.h";

@class UserDefaults;
@class Symbol;
@class Feed;
@class Chart;
@class QFields;
@protocol SymbolsDataDelegate;
@protocol mTraderStatusDelegate;

enum {
	HEADER = 0,
	FIXEDLENGTH,
	STATICRESPONSE,
	CHART,
	CONTENTLENGTH,
	LOGIN,
	PREPROCESSING,
	PROCESSING,
	NEWSFEEDS,
	NEWSLIST,
	NEWSITEM,
	QUOTE,
	QUOTES,
	STATIC,
	SEARCHRESULTS,
	ADDSEC,
	REMSEC,
	STATDATA,
	HISTDATA,
	KICKOUT
} states;

@interface mTraderCommunicator : NSObject <mTraderBlockDataDelegate> {
@private
	id <SymbolsDataDelegate> symbolsDelegate;
	id <mTraderStatusDelegate> _statusDelegate;

	UserDefaults *_defaults;
	
	BOOL symbolsDefined;
	
	NSData *_currentLine;
	NSMutableArray *_blockBuffer;
	QFields *_qFields;
	
	NSUInteger contentLength;
	NSUInteger state;
}

@property (nonatomic, assign) id <SymbolsDataDelegate> symbolsDelegate;
@property (nonatomic, assign) id <mTraderStatusDelegate> statusDelegate;
@property (nonatomic, retain) Communicator *communicator;
@property (nonatomic, assign) UserDefaults *defaults;
@property (nonatomic, retain) NSData *currentLine;
@property (nonatomic, retain) NSMutableArray *blockBuffer;
@property (nonatomic, retain) QFields *qFields;
@property (nonatomic, assign) NSUInteger state;
@property (nonatomic, assign) NSUInteger contentLength;

// The singleton class method
+ (mTraderCommunicator *)sharedManager;

// mTrader server request methods
- (void)login;
- (void)logout;
- (BOOL)loginStatusHasChanged;

- (void)newsListFeed:(NSString *)mCode;
- (void)newsItemRequest:(NSString *)newsId;

- (void)symbolSearch:(NSString *)symbol;
- (void)addSecurity:(NSString *)tickerSymbol withMCode:(NSString *)mCode;
- (void)removeSecurity:(NSString *)feedTicker;
- (void)setStreamingForFeedTicker:(NSString *)feedTicker;
- (void)staticDataForFeedTicker:(NSString *)feedTicker;
- (void)tradesRequest:(NSString *)feedTicker;
- (void)symbolNewsForFeedTicker:(NSString *)feedTicker;
- (void)graphForFeedTicker:(NSString *)feedTicker period:(NSUInteger)period width:(NSUInteger)width height:(NSUInteger)height orientation:(NSString *)orientation;
@end

@protocol mTraderStatusDelegate <NSObject>
- (void)connected;
- (void)disconnected;
- (void)kickedOut;
- (void)loginSuccessful;
- (void)loginFailed:(NSString *)message;
@end

@protocol SymbolsDataDelegate <NSObject>
- (void)replaceAllSymbols:(NSString *)symbols;
- (void)searchResults:(NSArray *)results;
- (void)addSymbols:(NSString *)symbols;
- (void)updateSymbols:(NSArray *)symbols;
- (void)staticUpdates:(NSDictionary *)updateDictionary;
- (void)tradesUpdate:(NSDictionary *)updateDictionary;
- (void)addExchanges:(NSArray *)exchanges;
- (void)addNewsFeeds:(NSArray *)feeds;
- (void)failedToAddNoSuchSecurity;
- (void)failedToAddAlreadyExists;
- (void)chartUpdate:(NSDictionary *)chart;
- (void)newsListFeedsUpdates:(NSArray *)newsList;
- (void)newsItemUpdate:(NSArray *)newsItemContents;
@optional
- (void)removedSecurity:(NSString *)feedTicker;
@end