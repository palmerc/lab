//
//  StockDetailController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "OrderBookController.h"
#import "TradesController.h"
#import "SymbolNewsController.h"
#import "ChartController.h"
#import "mTraderCommunicator.h"
@class Symbol;

@interface SymbolDetailController : UIViewController <SymbolsDataDelegate, OrderBookControllerDelegate, TradesControllerDelegate, SymbolNewsControllerDelegate, ChartControllerDelegate> {
@private
	NSManagedObjectContext *managedObjectContext;
	
	Symbol *_symbol;
	
	UIScrollView *scrollView;
	
	UIView *symbolsHeaderView;
	UIView *tradesHeaderView;
	UIView *fundamentalsHeaderView;

	UILabel *tickerName;
	UILabel *type;
	UILabel *isin;
	UILabel *currency;
	UILabel *country;
	
	UILabel *lastTrade;
	UILabel *lastTradeTime;
	UILabel *lastTradeChange;
	UILabel *lastTradePercentChange;
	UILabel *vwap;
	UILabel *open;
	UILabel *turnover;
	UILabel *high;
	UILabel *volume;
	UILabel *low;
	UILabel *buyLot;
	UILabel *buyLotValue;
	UILabel *trades;
	UILabel *tradingStatus;
	UILabel *averageValue;
	UILabel *averageVolume;
	UILabel *onVolume;
	
	UILabel *segment;
	UILabel *marketCapitalization;
	UILabel *outstandingShares;
	UILabel *dividend;
	UILabel *dividendDate;
	
	UIToolbar *toolBar;
	
	UIFont *headerFont;
	UIFont *mainFont;
	
	CGFloat globalY;
	
	NSDateFormatter *dateFormatter;
	NSDateFormatter *timeFormatter;
	NSNumberFormatter *doubleFormatter;
	NSNumberFormatter *integerFormatter;
	NSNumberFormatter *percentFormatter;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UIToolbar *toolBar;

- (id)initWithSymbol:(Symbol *)symbol;

- (void)orderBook:(id)sender;
- (void)setLabelFrame:(UILabel *)label;
- (void)setLeftLabelFrame:(UILabel *)leftLabel andRightLabelFrame:(UILabel *)rightLabel;

- (UILabel *)generateLabel;

- (void)setupPage;
- (void)updateSymbolInformation;
- (void)updateTradesInformation;
- (void)updateFundamentalsInformation;

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;

@end

