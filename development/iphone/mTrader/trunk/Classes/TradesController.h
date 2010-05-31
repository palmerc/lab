//
//  TradesController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "mTraderCommunicator.h"

@protocol TradesControllerDelegate;
@class Symbol;
@class TradesCell;

@interface TradesController : UITableViewController {
@private
	NSManagedObjectContext *_managedObjectContext;
	
	Symbol *_symbol;
	NSArray *_trades;
	
	UILabel *_tradesAvailableLabel;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSArray *trades;
@property (nonatomic, retain) UILabel *tradesAvailableLabel;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)configureCell:(TradesCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (void)updateTrades;
@end

