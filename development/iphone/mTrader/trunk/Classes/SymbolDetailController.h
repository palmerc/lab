//
//  StockDetailController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "OrderBookModalController.h"
#import "TradesController.h"
#import "SymbolNewsModalController.h"
#import "ChartController.h"
#import "mTraderCommunicator.h"

@class Symbol;
@class LastChangeView;
@class TradesInfoView;
@class OrderBookView;
@class SymbolNewsView;

@interface SymbolDetailController : UIViewController <SymbolsDataDelegate, OrderBookModalControllerDelegate, TradesControllerDelegate, SymbolNewsModalControllerDelegate, ChartControllerDelegate> {
@private
	NSManagedObjectContext *managedObjectContext;
	
	Symbol *_symbol;
	
	LastChangeView *lastBox;
	TradesInfoView *tradesBox;
	OrderBookView *orderBox;
	SymbolNewsView *newsBox;
	
	UIScrollView *scrollView;
		
	UIView *symbolsHeaderView;
	UIView *tradesHeaderView;
	UIView *fundamentalsHeaderView;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;

- (id)initWithSymbol:(Symbol *)symbol;

@end

