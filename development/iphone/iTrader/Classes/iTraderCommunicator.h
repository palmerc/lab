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
@protocol SymbolsDataDelegate;
@protocol StockAddDelegate;

@interface iTraderCommunicator : NSObject <CommunicatorReceiveDelegate> {
	id <SymbolsDataDelegate> symbolsDelegate;
	id <StockAddDelegate> stockAddDelegate;

	Communicator *_communicator;
	UserDefaults *_defaults;
	BOOL _isLoggedIn;
	BOOL _loginStatusHasChanged;
}

@property (nonatomic, assign) id <SymbolsDataDelegate> symbolsDelegate;
@property (nonatomic, assign) id <StockAddDelegate> stockAddDelegate;
@property (nonatomic, retain) Communicator *communicator;
@property (nonatomic, retain) UserDefaults *defaults;
@property (readonly) BOOL isLoggedIn;

+ (iTraderCommunicator *)sharedManager;

- (void)login;
- (void)logout;
- (void)addSecurity:(NSString *)tickerSymbol;
- (void)removeSecurity:(NSString *)feedTicker;
- (BOOL)loginStatusHasChanged;
- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings;
- (NSArray *)stripOffFirstElement:(NSArray *)array;
@end

@protocol SymbolsDataDelegate <NSObject>
- (void)addSymbol:(Symbol *)symbol withFeed:(Feed *)feed;
- (void)updateQuotes:(NSArray *)quotes;
@end

@protocol StockAddDelegate <NSObject>
- (void)addOK;
- (void)addFailedAlreadyExists;
- (void)addFailedNotFound;
@end