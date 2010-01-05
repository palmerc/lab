//
//  MyStocksViewController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SymbolsController.h"

@class iTraderCommunicator;
@class SymbolsController;

@interface MyStocksViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, SymbolsUpdateDelegate> {
	iTraderCommunicator *communicator;
	SymbolsController *symbolsController;
}

@end
