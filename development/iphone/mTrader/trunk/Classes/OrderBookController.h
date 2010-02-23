//
//  OrderBookController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
@class Symbol;

@interface OrderBookController : UITableViewController <NSFetchedResultsControllerDelegate, SymbolsDataDelegate> {
@private
	NSManagedObjectContext *managedObjectContext;

	Symbol *_symbol;
	
	NSMutableArray *asks;
	NSMutableArray *bids;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSArray *asks;
@property (nonatomic, retain) NSArray *bids;

- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (NSArray *)fetchOrderBookForSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;

@end
