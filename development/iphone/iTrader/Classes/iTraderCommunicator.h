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
@protocol mTraderServerDataDelegate;
@protocol StockAddDelegate;

enum {
	START = 0,
	STATICDATA = 1,
	STREAMINGDATA = 2
} states;

@interface iTraderCommunicator : NSObject <CommunicatorReceiveDelegate> {
	id <mTraderServerDataDelegate> mTraderServerDataDelegate;
	id <StockAddDelegate> stockAddDelegate;

	Communicator *_communicator;
	UserDefaults *_defaults;
	BOOL _isLoggedIn;
	BOOL _loginStatusHasChanged;
	
	NSMutableArray *_blockBuffer;
	NSUInteger contentLength;
	NSUInteger state;
}

@property (nonatomic, assign) id <mTraderServerDataDelegate> mTraderServerDataDelegate;
@property (nonatomic, assign) id <StockAddDelegate> stockAddDelegate;
@property (nonatomic, retain) Communicator *communicator;
@property (nonatomic, retain) UserDefaults *defaults;
@property (readonly) BOOL isLoggedIn;
@property (nonatomic, retain) NSMutableArray *blockBuffer;

+ (iTraderCommunicator *)sharedManager;

- (void)login;
- (void)logout;
- (void)addSecurity:(NSString *)tickerSymbol;
- (void)removeSecurity:(NSString *)feedTicker;
- (BOOL)loginStatusHasChanged;
- (void)graphForFeedTicker:(NSString *)feedTicker period:(NSUInteger)period width:(NSUInteger)width height:(NSUInteger)height orientation:(NSString *)orientation;

// Helper methods
- (void)handleEvent;
- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings;
- (NSArray *)stripOffFirstElement:(NSArray *)array;
- (NSString *)cleanString:(NSString *)string;
- (NSArray *)cleanStrings:(NSArray *)strings;
@end

@protocol mTraderServerDataDelegate <NSObject>
- (void)addFeed:(Feed *)feed;
- (void)addSymbol:(Symbol *)symbol;
- (void)addSymbol:(Symbol *)symbol withFeed:(Feed *)feed;
- (void)updateQuotes:(NSArray *)quotes;
@end

@protocol StockAddDelegate <NSObject>
- (void)addOK;
- (void)addFailedAlreadyExists;
- (void)addFailedNotFound;
@end