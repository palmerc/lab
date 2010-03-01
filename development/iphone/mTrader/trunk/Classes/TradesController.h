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

@interface TradesController : UIViewController <SymbolsDataDelegate, UITableViewDelegate, UITableViewDataSource> {
@private
	id <TradesControllerDelegate> delegate;
	
	UITableView *table;
	UIView *tradeTimeLabel;
	UIView *tradePriceLabel;
	UIView *tradeVolumeLabel;
	
	Symbol *_symbol;
	NSArray *_trades;
}

@property (assign) id <TradesControllerDelegate> delegate;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSArray *trades;

- (UIView *)setHeader:(NSString *)header withFrame:(CGRect)frame;
- (void)configureCell:(TradesCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
@end


@protocol TradesControllerDelegate
- (void)tradesControllerDidFinish:(TradesController *)controller;
@end

