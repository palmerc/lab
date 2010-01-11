//
//  MyStocksViewController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SymbolsController.h"
#import "StockSearchController.h"

@class iTraderCommunicator;
@class SymbolsController;

typedef enum {
	NOCHANGE = 0,
	UP = 1,
	DOWN = 2
} changeEnum;

@interface MyStocksViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, StockSearchControllerDelegate, SymbolsUpdateDelegate> {
	iTraderCommunicator *_communicator;
	SymbolsController *_symbolsController;
}

@property (nonatomic, retain) SymbolsController *symbolsController;

- (void)addStockButtonWasPressed:(id)sender;

@end
