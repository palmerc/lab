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
@protocol SymbolsDataDelegate;
@protocol mTraderServerDataDelegate;
@protocol StockAddDelegate;
@protocol NewsItemDataDelegate;
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
	NEWSITEM,
	QUOTE,
	QUOTES,
	STATIC,
	ADDSEC,
	REMSEC,
	STATDATA,
	KICKOUT
} states;

@interface mTraderCommunicator : NSObject <CommunicatorDataDelegate> {
	id <SymbolsDataDelegate> symbolsDelegate;
	id <mTraderServerDataDelegate> mTraderServerDataDelegate;
	id <mTraderServerMonitorDelegate> mTraderServerMonitorDelegate;
	id <NewsItemDataDelegate> newsItemDelegate;

	Communicator *_communicator;
	UserDefaults *_defaults;
	
	BOOL isLoggedIn;
	BOOL loginStatusHasChanged;
	BOOL symbolsDefined;
	
	NSData *_currentLine;
	NSMutableArray *_blockBuffer;
	NSUInteger contentLength;
	NSUInteger state;
@private
	NSString *_server;
	NSString *_port;
}

@property (nonatomic, assign) id <SymbolsDataDelegate> symbolsDelegate;
@property (nonatomic, assign) id <mTraderServerDataDelegate> mTraderServerDataDelegate;
@property (nonatomic, assign) id <NewsItemDataDelegate> newsItemDelegate;
@property (nonatomic, assign) id <mTraderServerMonitorDelegate> mTraderServerMonitorDelegate;
@property (nonatomic, retain) Communicator *communicator;
@property (nonatomic, assign) UserDefaults *defaults;
@property (readonly) BOOL isLoggedIn;
@property (nonatomic, retain) NSData *currentLine;
@property (nonatomic, retain) NSMutableArray *blockBuffer;
@property (nonatomic, assign) NSUInteger state;
@property (nonatomic, assign) NSUInteger contentLength;
@property (nonatomic, assign) NSString *server;
@property (nonatomic, assign) NSString *port;

// The singleton class method
+ (mTraderCommunicator *)sharedManager;
- (id)initWithURL:(NSString *)url onPort:(NSInteger)port;

// mTrader server request methods
- (void)login;
- (void)logout;
-(void) newsListFeeds;
-(void) newsItemRequest:(NSString *)newsId;
-(void) addSecurity:(NSString *)tickerSymbol withMCode:(NSString *)mCode;
- (void)orderBookForFeedTicker:(NSString *)feedTicker;
- (void)removeSecurity:(NSString *)feedTicker;
- (BOOL)loginStatusHasChanged;
- (void)staticDataForFeedTicker:(NSString *)feedTicker;
- (void)graphForFeedTicker:(NSString *)feedTicker period:(NSUInteger)period width:(NSUInteger)width height:(NSUInteger)height orientation:(NSString *)orientation;
- (void)stopStreamingData;

// State machine methods
-(void) stateMachine;
-(void) headerParsing;
-(void) fixedLength;
-(void) staticResponse;
- (void)chartHandling;
- (void)loginHandling;
- (void)preprocessing;
- (void)processingLoop;
- (void)quoteHandling;
- (void)addSecurityOK;
- (void)removeSecurityOK;
-(void) newsListFeedsOK;
-(void) newsBodyOK;
- (void)staticDataOK;

// Parsing methods
- (NSArray *)quotesParsing:(NSString *)quotes;
-(NSArray *)exchangesParsing:(NSString *)exchanges;
//- (void)symbolsParsing:(NSString *)symbols;
- (void)staticDataParsing:(NSString *)secOid;

// Helper methods
- (NSString *)dataFromRHS:(NSString *)string;
- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings;
- (NSArray *)stripOffFirstElement:(NSArray *)array;
- (NSString *)dataToString:(NSData *)data;
- (NSString *)cleanString:(NSString *)string;
- (NSArray *)cleanStrings:(NSArray *)strings;
@end

@protocol mTraderServerMonitorDelegate <NSObject>
-(void) kickedOut;
@end

@protocol SymbolsDataDelegate <NSObject>
- (void)replaceAllSymbols:(NSString *)symbols;
- (void)addSymbols:(NSString *)symbols;
- (void)updateSymbols:(NSArray *)symbols;
- (void)staticUpdates:(NSDictionary *)updateDictionary;
- (void)addExchanges:(NSArray *)exchanges;
- (void)failedToAddNoSuchSecurity;
- (void)failedToAddAlreadyExists;
- (void)chartUpdate:(NSDictionary *)chart;
@end


// Delegate Protocols
@protocol mTraderServerDataDelegate <NSObject>
@optional
- (void)addFeed:(Feed *)feed;
- (void)addSymbol:(Symbol *)symbol;
- (void)addSymbol:(Symbol *)symbol withFeed:(Feed *)feed;
- (void)addExchanges:(NSArray *)exchanges;
- (void)removedSecurity:(NSString *)feedTicker;
- (void)newsListFeedsUpdates:(NSArray *)newsList;
@end

@protocol NewsItemDataDelegate <NSObject>
-(void) newsItemUpdate:(NSArray *)newsItemContents;
@end;
