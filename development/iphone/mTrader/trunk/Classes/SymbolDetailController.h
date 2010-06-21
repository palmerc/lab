//
//  StockDetailController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "OrderBookModalController.h"
#import "TradesModalController.h"
#import "SymbolNewsModalController_Phone.h"

#import "ChartController.h"
#import "mTraderCommunicator.h"

@class Symbol;
@class LastChangeView;
@class TradesLiveInfoView;
@class TradesInfoView;
@class OrderBookView;
@class SymbolNewsView;
@class OtherInfoView;
@class ScrollViewPageControl;

@interface SymbolDetailController : UIViewController <OrderBookModalControllerDelegate, TradesControllerDelegate, SymbolNewsModalControllerDelegate, ChartControllerDelegate> {
@private
	NSManagedObjectContext *_managedObjectContext;
	
	Symbol *_symbol;
	
	LastChangeView *_lastBox;
	TradesLiveInfoView *_tradesLiveBox;
	TradesInfoView *_tradesBox;
	OrderBookView *_orderBox;
	SymbolNewsView *_newsBox;
	ScrollViewPageControl *_detailBox;
	OtherInfoView *_otherBox;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;

- (id)initWithSymbol:(Symbol *)symbol;
- (void)changeQFieldsStreaming;

@end

