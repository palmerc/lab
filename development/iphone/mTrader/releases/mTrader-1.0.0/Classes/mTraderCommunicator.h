//
//  mTraderCommunicator.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "Communicator.h";

@class UserDefaults;
@class Symbol;
@class Feed;
@class Chart;
@class QFields;
@protocol SymbolsDataDelegate;
@protocol mTraderServerMonitorDelegate;

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
	ADDSEC,
	REMSEC,
	STATDATA,
	HISTDATA,
	KICKOUT
} states;

@interface mTraderCommunicator : NSObject <CommunicatorDataDelegate> {
@private
	id <SymbolsDataDelegate> symbolsDelegate;
	id <mTraderServerMonitorDelegate> mTraderServerMonitorDelegate;

	Communicator *_communicator;
	UserDefaults *_defaults;
	
	BOOL isLoggedIn;
	BOOL loginStatusHasChanged;
	BOOL symbolsDefined;
	
	NSData *_currentLine;
	NSMutableArray *_blockBuffer;
	QFields *_qFields;
	
	NSUInteger contentLength;
	NSUInteger state;
}

@property (nonatomic, assign) id <SymbolsDataDelegate> symbolsDelegate;
@property (nonatomic, assign) id <mTraderServerMonitorDelegate> mTraderServerMonitorDelegate;
@property (nonatomic, retain) Communicator *communicator;
@property (nonatomic, assign) UserDefaults *defaults;
@property (readonly) BOOL isLoggedIn;
@property (nonatomic, retain) NSData *currentLine;
@property (nonatomic, retain) NSMutableArray *blockBuffer;
@property (nonatomic, retain) QFields *qFields;
@property (nonatomic, assign) NSUInteger state;
@property (nonatomic, assign) NSUInteger contentLength;

// The singleton class method
+ (mTraderCommunicator *)sharedManager;
- (id)initWithURL:(NSString *)url onPort:(NSInteger)port;

// mTrader server request methods
- (void)login;
- (void)logout;
- (BOOL)loginStatusHasChanged;

- (void)newsListFeed:(NSString *)mCode;
- (void)newsItemRequest:(NSString *)newsId;

- (void)addSecurity:(NSString *)tickerSymbol withMCode:(NSString *)mCode;
- (void)removeSecurity:(NSString *)feedTicker;
- (void)setStreamingForFeedTicker:(NSString *)feedTicker;
- (void)staticDataForFeedTicker:(NSString *)feedTicker;
- (void)tradesRequest:(NSString *)feedTicker;
- (void)symbolNewsForFeedTicker:(NSString *)feedTicker;
- (void)graphForFeedTicker:(NSString *)feedTicker period:(NSUInteger)period width:(NSUInteger)width height:(NSUInteger)height orientation:(NSString *)orientation;

// State machine methods
- (void)stateMachine;
- (void)headerParsing;
- (void)fixedLength;
- (void)staticResponse;
- (void)chartHandling;
- (void)loginHandling;
- (void)preprocessing;
- (void)processingLoop;
- (void)quoteHandling;
- (void)addSecurityOK;
- (void)removeSecurityOK;
- (void)newsListFeedsOK;
- (void)newsListOK;
- (void)newsBodyOK;
- (void)staticDataOK;
- (void)historyDataOK;

// Parsing methods
- (NSArray *)quotesParsing:(NSString *)quotes;
- (NSArray *)exchangesParsing:(NSString *)exchanges;
- (void)staticDataParsing:(NSString *)secOid;
- (void)historyDataParsing:(NSString *)secOid;

// Helper methods
- (NSString *)dataFromRHS:(NSString *)string;
- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings;
- (NSArray *)stripOffFirstElement:(NSArray *)array;
- (NSString *)dataToString:(NSData *)data;
@end

@protocol mTraderServerMonitorDelegate <NSObject>
- (void)disconnected;
- (void)kickedOut;
@end

@protocol SymbolsDataDelegate <NSObject>
@optional
- (void)replaceAllSymbols:(NSString *)symbols;
- (void)addSymbols:(NSString *)symbols;
- (void)updateSymbols:(NSArray *)symbols;
- (void)staticUpdates:(NSDictionary *)updateDictionary;
- (void)tradesUpdate:(NSDictionary *)updateDictionary;
- (void)addExchanges:(NSArray *)exchanges;
- (void)addNewsFeeds:(NSArray *)feeds;
- (void)failedToAddNoSuchSecurity;
- (void)failedToAddAlreadyExists;
- (void)removedSecurity:(NSString *)feedTicker;
- (void)chartUpdate:(NSDictionary *)chart;
- (void)newsListFeedsUpdates:(NSArray *)newsList;
- (void)newsItemUpdate:(NSArray *)newsItemContents;
@end