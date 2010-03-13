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
@class LastChangeView;
@class TradesInfoView;
@class OrderBookView;

@interface SymbolDetailController : UIViewController <SymbolsDataDelegate, OrderBookControllerDelegate, TradesControllerDelegate, SymbolNewsControllerDelegate, ChartControllerDelegate> {
@private
	NSManagedObjectContext *managedObjectContext;
	
	Symbol *_symbol;
	
	LastChangeView *lastBox;
	TradesInfoView *tradesBox;
	OrderBookView *orderBox;

	
	UIScrollView *scrollView;
		
	UIView *symbolsHeaderView;
	UIView *tradesHeaderView;
	UIView *fundamentalsHeaderView;

	UILabel *tickerName;
	UILabel *typeLabel;
	UILabel *type;
	UILabel *isinLabel;
	UILabel *isin;
	UILabel *currencyLabel;
	UILabel *currency;
	UILabel *countryLabel; 
	UILabel *country;
	
	UILabel *lastTradeLabel;
	UILabel *lastTrade;
	UILabel *lastTradeTimeLabel;
	UILabel *lastTradeTime;
	UILabel *lastTradeChangeLabel;
	UILabel *lastTradeChange;
	UILabel *lastTradePercentChangeLabel;
	UILabel *lastTradePercentChange;
	UILabel *vwapLabel;
	UILabel *vwap;
	UILabel *openLabel;
	UILabel *open;
	UILabel *turnoverLabel;
	UILabel *turnover;
	UILabel *highLabel;
	UILabel *high;
	UILabel *volumeLabel;
	UILabel *volume;
	UILabel *lowLabel;
	UILabel *low;
	UILabel *buyLotLabel;
	UILabel *buyLot;
	UILabel *buyLotValueLabel;
	UILabel *buyLotValue;
	UILabel *tradesLabel;
	UILabel *trades;
	UILabel *tradingStatusLabel;
	UILabel *tradingStatus;
	UILabel *averageValueLabel;
	UILabel *averageValue;
	UILabel *averageVolumeLabel;
	UILabel *averageVolume;
	UILabel *onVolumeLabel;
	UILabel *onVolume;
	
	UILabel *segmentLabel;
	UILabel *segment;
	UILabel *marketCapitalizationLabel;
	UILabel *marketCapitalization;
	UILabel *outstandingSharesLabel;
	UILabel *outstandingShares;
	UILabel *dividendLabel;
	UILabel *dividend;
	UILabel *dividendDateLabel;
	UILabel *dividendDate;
	
	UIToolbar *toolBar;
	
	UIFont *lastFont;
	UIFont *headerFont;
	UIFont *mainFont;
	UIFont *mainFontBold;
	
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
- (void)setLeftLabelFrame:(UILabel *)leftLabel andLeftData:(UILabel *)leftDataLabel andRightLabelFrame:(UILabel *)rightLabel andRightData:(UILabel *)rightDataLabel;

- (UILabel *)generateLabelWithText:(NSString *)text;

- (void)setupPage;
- (void)updateSymbolInformation;
- (void)updateTradesInformation;
- (void)updateFundamentalsInformation;

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;

@end

