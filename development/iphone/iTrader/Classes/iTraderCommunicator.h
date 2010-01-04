//
//  mTraderCommunicator.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Communicator.h";
@class Symbol;
@class Feed;
@protocol SymbolsDataDelegate;

@interface iTraderCommunicator : NSObject <CommunicatorReceiveDelegate> {
	id <SymbolsDataDelegate> symbolsDelegate;

	Communicator *_communicator;
	BOOL _isLoggedIn;
	BOOL _loginStatusHasChanged;
}

@property (nonatomic, assign) id <SymbolsDataDelegate> symbolsDelegate;
@property (nonatomic, retain) Communicator *communicator;
@property (readonly) BOOL isLoggedIn;

+ (iTraderCommunicator *)sharedManager;

- (void)login;
- (void)logout;
- (BOOL)loginStatusHasChanged;
- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings;
@end

@protocol SymbolsDataDelegate <NSObject>
- (void)addSymbol:(Symbol *)symbol;
- (void)addFeed:(Feed *)feed;
@end