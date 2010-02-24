//
//  OrderBookController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
@protocol OrderBookControllerDelegate;
@class Symbol;

@interface OrderBookController : UIViewController <NSFetchedResultsControllerDelegate, SymbolsDataDelegate, UITableViewDataSource, UITableViewDelegate> {
@private
	NSManagedObjectContext *managedObjectContext;

	Symbol *_symbol;
	
	id <OrderBookControllerDelegate> delegate;
	
	UITableView *table;
	UILabel *askSizeLabel;
	UILabel *askValueLabel;
	UILabel *bidSizeLabel;
	UILabel *bidValueLabel;
	
	NSMutableArray *asks;
	NSMutableArray *bids;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@property (assign) id <OrderBookControllerDelegate> delegate;
@property (nonatomic, retain) UITableView *table;
@property (nonatomic, retain) NSArray *asks;
@property (nonatomic, retain) NSArray *bids;

- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (UILabel *)generateLabel;

@end

@protocol OrderBookControllerDelegate
- (void)orderBookControllerDidFinish:(OrderBookController *)controller;
@end
