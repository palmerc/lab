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
	iTraderCommunicator *communicator;
	SymbolsController *symbolsController;
	
	NSMutableArray *_sections; // The index is the section number and the data should be an individual feedNumber
	NSMutableArray *_rows; // The index is the row number 
	
	BOOL firstUpdate;
}

@property (nonatomic, retain) NSMutableArray *sections;
@property (nonatomic, retain) NSMutableArray *rows;

- (void)addStock:(id)sender;

@end
