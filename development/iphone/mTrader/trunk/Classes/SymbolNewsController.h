//
//  SymbolNewsController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@class Symbol;
@protocol SymbolNewsControllerDelegate;

@interface SymbolNewsController : UIViewController <SymbolsDataDelegate> {
@private
	id <SymbolNewsControllerDelegate> delegate;
	
	Symbol *_symbol;
	
	NSArray *newsArray;
	
	UITableView *table;
}

@property (assign) id <SymbolNewsControllerDelegate> delegate;
@property (nonatomic, retain) Symbol *symbol;

- (id)initWithSymbol:(Symbol *)symbol;
@end

@protocol SymbolNewsControllerDelegate
- (void)symbolNewsControllerDidFinish:(SymbolNewsController *)controller;
@end