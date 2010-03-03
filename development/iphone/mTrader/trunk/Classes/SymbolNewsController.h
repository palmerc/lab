//
//  SymbolNewsController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@class Symbol;
@class SymbolNewsCell;
@protocol SymbolNewsControllerDelegate;

@interface SymbolNewsController : UIViewController <SymbolsDataDelegate, UITableViewDelegate, UITableViewDataSource> {
@private
	id <SymbolNewsControllerDelegate> delegate;
	mTraderCommunicator *communicator;
	
	Symbol *_symbol;
	NSArray *newsArray;
	
	UITableView *table;
}

@property (assign) id <SymbolNewsControllerDelegate> delegate;
@property (assign) mTraderCommunicator *communicator;
@property (nonatomic, retain) Symbol *symbol;

- (id)initWithSymbol:(Symbol *)symbol;
- (void)configureCell:(SymbolNewsCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end

@protocol SymbolNewsControllerDelegate
- (void)symbolNewsControllerDidFinish:(SymbolNewsController *)controller;
@end