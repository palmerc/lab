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

enum {
	CHART = 0,
	CONTENTLENGTH = 1,
	LOGIN = 2,
	PREPROCESSING = 3,
	PROCESSING = 4,
	QUOTE = 5,
	QUOTES = 6,
	STATIC = 7,
	ADDSEC = 8,
	REMSEC = 9,
	STATDATA = 10,
	KICKOUT = 11
} states;

@interface iTraderCommunicator : NSObject <CommunicatorDataDelegate> {
	id <mTraderServerDataDelegate> mTraderServerDataDelegate;
	id <StockAddDelegate> stockAddDelegate;

	Communicator *_communicator;
	UserDefaults *_defaults;
	
	BOOL isLoggedIn;
	BOOL loginStatusHasChanged;
	
	NSData *_currentLine;
	NSMutableArray *_blockBuffer;
	NSUInteger contentLength;
	NSUInteger state;
}

@property (nonatomic, assign) id <mTraderServerDataDelegate> mTraderServerDataDelegate;
@property (nonatomic, assign) id <StockAddDelegate> stockAddDelegate;
@property (nonatomic, retain) Communicator *communicator;
@property (nonatomic, retain) UserDefaults *defaults;
@property (readonly) BOOL isLoggedIn;
@property (nonatomic, retain) NSData *currentLine;
@property (nonatomic, retain) NSMutableArray *blockBuffer;

// The singleton class method
+ (iTraderCommunicator *)sharedManager;

// mTrader server request methods
- (void)login;
- (void)logout;
- (void)addSecurity:(NSString *)tickerSymbol;
- (void)removeSecurity:(NSString *)feedTicker;
- (BOOL)loginStatusHasChanged;
- (void)staticDataForFeedTicker:(NSString *)feedTicker;
- (void)graphForFeedTicker:(NSString *)feedTicker period:(NSUInteger)period width:(NSUInteger)width height:(NSUInteger)height orientation:(NSString *)orientation;

// State machine methods
- (void)chartHandling;
- (void)contentLength;
- (void)loginHandling;
- (void)preprocessing;
- (void)processingLoop;
- (void)quoteHandling;
- (void)staticLoop;
- (void)addSecurityOK;
- (void)removeSecurityOK;
- (void)staticDataOK;

// Parsing methods
- (Chart *)chartParsing;
- (NSArray *)quotesParsing:(NSString *)quotes;
- (void)symbolsParsing:(NSString *)symbols;
- (void)settingsParsing;
- (void)staticDataParsing;

// Helper methods
- (void)blockBufferRenew;
- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings;
- (NSArray *)stripOffFirstElement:(NSArray *)array;
- (NSString *)dataToString:(NSData *)data;
- (NSString *)currentLineToString;
- (NSString *)cleanString:(NSString *)string;
- (NSArray *)cleanStrings:(NSArray *)strings;
@end

// Delegate Protocols
@protocol mTraderServerDataDelegate <NSObject>
- (void)chart:(Chart *)chart;
- (void)addFeed:(Feed *)feed;
- (void)addSymbol:(Symbol *)symbol;
- (void)addSymbol:(Symbol *)symbol withFeed:(Feed *)feed;
- (void)updateQuotes:(NSArray *)quotes;
- (void)staticUpdates:(NSDictionary *)updateDictionary;
@end

@protocol StockAddDelegate <NSObject>
- (void)addOK;
- (void)addFailedAlreadyExists;
- (void)addFailedNotFound;
@end