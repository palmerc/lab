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
	id <SymbolsDataDelegate> _symbolsDelegate;
	id <mTraderStatusDelegate> _statusDelegate;

	UserDefaults *_defaults;
		
	NSData *_currentLine;
	NSMutableArray *_blockBuffer;
	QFields *_qFields;
	
	NSUInteger _contentLength;
	NSUInteger _state;
}

@property (nonatomic, assign) id <SymbolsDataDelegate> symbolsDelegate;
@property (nonatomic, assign) id <mTraderStatusDelegate> statusDelegate;
@property (nonatomic, retain) QFields *qFields;

// The singleton class method
+ (mTraderCommunicator *)sharedManager;

// mTrader server request methods
- (void)login;
- (void)logout;

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
@optional
- (void)connected;
- (void)disconnected;
- (void)kickedOut;
- (void)loginSuccessful;
- (void)loginFailed:(NSString *)message;
@end

@protocol SymbolsDataDelegate <NSObject>
@optional
- (void)processSymbolFeeds:(NSArray *)symbolFeeds;
- (void)processSymbols:(NSArray *)symbols;
- (void)processNewsFeeds:(NSArray *)newsFeeds;
- (void)replaceAllSymbols:(NSString *)symbols;
- (void)searchResults:(NSArray *)results;
- (void)updateSymbols:(NSArray *)symbols;
- (void)staticUpdates:(NSDictionary *)updateDictionary;
- (void)tradesUpdate:(NSDictionary *)updateDictionary;
- (void)failedToAddNoSuchSecurity;
- (void)failedToAddAlreadyExists;
- (void)chartUpdate:(NSDictionary *)chart;
- (void)newsListFeedsUpdates:(NSArray *)newsList;
- (void)newsItemUpdate:(NSArray *)newsItemContents;
- (void)removedSecurity:(NSString *)feedTicker;
@end