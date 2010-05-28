//
//  TradesModalController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "mTraderCommunicator.h"

@protocol TradesControllerDelegate;
@class Symbol;
@class TradesCell;

@interface TradesModalController : UIViewController <UITableViewDelegate, UITableViewDataSource> {
@private
	id <TradesControllerDelegate> delegate;
	NSManagedObjectContext *_managedObjectContext;
	
	UITableView *table;
	UIView *tradeTimeLabel;
	UIView *tradePriceLabel;
	UIView *tradeVolumeLabel;
	
	Symbol *_symbol;
	NSArray *_trades;
}

@property (assign) id <TradesControllerDelegate> delegate;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSArray *trades;

- (UIView *)setHeader:(NSString *)header withFrame:(CGRect)frame;
- (void)configureCell:(TradesCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (void)updateTrades;
@end


@protocol TradesControllerDelegate
- (void)tradesControllerDidFinish:(TradesModalController *)controller;
@end

