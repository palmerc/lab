//
//  mTraderCommunicator.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Communicator.h";

@class UserDefaults;
@class Symbol;
@class Feed;
@class Chart;
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

@interface iTraderCommunicator : NSObject <CommunicatorDataDelegate> {
	id <mTraderServerDataDelegate> mTraderServerDataDelegate;
	id <mTraderServerMonitorDelegate> mTraderServerMonitorDelegate;
	id <StockAddDelegate> stockAddDelegate;
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
}

@property (nonatomic, assign) id <mTraderServerDataDelegate> mTraderServerDataDelegate;
@property (nonatomic, assign) id <StockAddDelegate> stockAddDelegate;
@property (nonatomic, assign) id <NewsItemDataDelegate> newsItemDelegate;
@property (nonatomic, assign) id <mTraderServerMonitorDelegate> mTraderServerMonitorDelegate;
@property (nonatomic, retain) Communicator *communicator;
@property (nonatomic, retain) UserDefaults *defaults;
@property (readonly) BOOL isLoggedIn;
@property (nonatomic, retain) NSData *currentLine;
@property (nonatomic, retain) NSMutableArray *blockBuffer;
@property (nonatomic, assign) NSUInteger state;
@property (nonatomic, assign) NSUInteger contentLength;

// The singleton class method
+ (iTraderCommunicator *)sharedManager;

// mTrader server request methods
- (void)login;
- (void)logout;
-(void) newsListFeeds;
-(void) newsItemRequest:(NSString *)newsId;
- (void)addSecurity:(NSString *)tickerSymbol;
- (void)removeSecurity:(NSString *)feedTicker;
- (BOOL)loginStatusHasChanged;
- (void)staticDataForFeedTicker:(NSString *)feedTicker;
- (void)graphForFeedTicker:(NSString *)feedTicker period:(NSUInteger)period width:(NSUInteger)width height:(NSUInteger)height orientation:(NSString *)orientation;

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
- (void)symbolsParsing:(NSString *)symbols;
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

// Delegate Protocols
@protocol mTraderServerDataDelegate <NSObject>
@optional
-(void) chart:(Chart *)chart;
-(void) addFeed:(Feed *)feed;
-(void) addSymbol:(Symbol *)symbol;
-(void) addSymbol:(Symbol *)symbol withFeed:(Feed *)feed;
-(void) updateQuotes:(NSArray *)quotes;
-(void) addExchanges:(NSArray *)exchanges;
-(void) staticUpdates:(NSDictionary *)updateDictionary;
-(void) removedSecurity:(NSString *)feedTicker;
-(void) newsListFeedsUpdates:(NSArray *)newsList;
@end

@protocol NewsItemDataDelegate <NSObject>
-(void) newsItemUpdate:(NSArray *)newsItemContents;
@end;

@protocol StockAddDelegate <NSObject>
- (void)addOK;
- (void)addFailedAlreadyExists;
- (void)addFailedNotFound;
@end