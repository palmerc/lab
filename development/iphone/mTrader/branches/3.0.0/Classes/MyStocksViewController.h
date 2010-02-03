//
//  MyStocksViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "SymbolsController.h"
#import "StockListingCell.h"
#import "StockSearchController.h"

@class mTraderCommunicator;
@class SymbolsController;

typedef enum {
	NOCHANGE = 0,
	UP,
	DOWN
} changeEnum;

typedef enum {
	PRICE = 0,
	PERCENT,
	VOLUME
} valueType;

@interface MyStocksViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, StockSearchControllerDelegate, SymbolsUpdateDelegate, TouchedValueButtonDelegate> {
	mTraderCommunicator *_communicator;
	SymbolsController *_symbolsController;
	
	NSUInteger currentValueType;
	BOOL _editing;
}

@property (nonatomic, retain) SymbolsController *symbolsController;
@property (assign) BOOL editing;
- (void)addStockButtonWasPressed:(id)sender;

@end
