//
//  StocksController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "iTraderCommunicator.h"

@protocol SymbolsUpdateDelegate;

@interface SymbolsController : NSObject <SymbolsDataDelegate> {
	id <SymbolsUpdateDelegate> updateDelegate;
	iTraderCommunicator *communicator;
	
	NSMutableDictionary *symbols; // A hash from feedTicker to the index in orderedSymbols Array
	NSMutableArray *orderedSymbols; // The index represents the row number for table views
	NSMutableDictionary *feeds; // A hash from feedNumber to index in orderedFeeds Array
	NSMutableArray *orderedFeeds; // This index represents the section number for table views
}

@property (assign) id <SymbolsUpdateDelegate> updateDelegate;
@property (nonatomic, retain) NSMutableDictionary *symbols;
@property (nonatomic, retain) NSMutableArray *orderedSymbols;
@property (nonatomic, retain) NSMutableDictionary *feeds;
@property (nonatomic, retain) NSMutableArray *orderedFeeds;

+ (SymbolsController *)sharedManager;
- (NSArray *)cleanQuote:(NSString *)quote;
- (Symbol *)symbolAtIndexPath:(NSIndexPath *)indexPath;

@end

@protocol SymbolsUpdateDelegate <NSObject>
-(void)symbolAdded:(Symbol *)symbol;
-(void)symbolsUpdated:(NSArray *)quotes;
@end


