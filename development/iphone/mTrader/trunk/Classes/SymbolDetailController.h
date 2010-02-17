//
//  StockDetailController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "ChainsTableViewController.h"
@class Symbol;
@class SymbolDetailPageOne;

@interface SymbolDetailController : UIViewController {
	Symbol *symbol;
	UIToolbar *toolBar;
	
	SymbolDetailPageOne *pageOneView;
	IBOutlet UIScrollView *scrollView;
//	IBOutlet UILabel *tickerSymbolLabel;
//	IBOutlet UILabel *nameLabel;
//	IBOutlet UILabel *typeLabel;
//	IBOutlet UILabel *statusLabel;
//	IBOutlet UILabel *stockISINLabel;
//	IBOutlet UILabel *lastTradeLabel;
//	IBOutlet UILabel *lastTradeChangeLabel;
//	IBOutlet UILabel *lastTradePercentChangeLabel;
//	IBOutlet UILabel *lastTradeTimeLabel;
//	IBOutlet UILabel *lowLabel;
//	IBOutlet UILabel *highLabel;
//	IBOutlet UILabel *volumeLabel;
//	IBOutlet UILabel *openLabel;
//	IBOutlet UILabel *previousCloseLabel;
//	IBOutlet UILabel *openChangeLabel;
//	IBOutlet UILabel *openPercentChangeLabel;
//	IBOutlet UILabel *vwapLabel;
//	IBOutlet UILabel *bidPriceLabel;
//	IBOutlet UILabel *bidSizeLabel;
//	IBOutlet UILabel *askPriceLabel;
//	IBOutlet UILabel *askSizeLabel;
//	IBOutlet UILabel *countryLabel;
//	IBOutlet UILabel *currencyLabel;
//	IBOutlet UILabel *outstandingSharesLabel;
//	IBOutlet UILabel *marketCapitalizationLabel;
//	IBOutlet UILabel *buyLotLabel;
//	IBOutlet UILabel *butLotValueLabel;
//	IBOutlet UILabel *turnoverLabel;
//	IBOutlet UILabel *onVolumeLabel;
//	IBOutlet UILabel *onValueLabel;
//	IBOutlet UILabel *averageVolumeLabel;
//	IBOutlet UILabel *averageValueLabel;
//	
//	IBOutlet UIButton *chart;
//	IBOutlet UIActivityIndicatorView *chartActivity;
//	NSUInteger period;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UIToolbar *toolBar;
@property (nonatomic, retain) IBOutlet UIScrollView *scrollView;
//@property (nonatomic, retain) IBOutlet UILabel *nameLabel;
//@property (nonatomic, retain) IBOutlet UILabel *tickerSymbolLabel;
//@property (nonatomic, retain) IBOutlet UILabel *statusLabel;
//@property (nonatomic, retain) IBOutlet UILabel *typeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *stockISINLabel;
//@property (nonatomic, retain) IBOutlet UILabel *lastTradeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *lastTradeChangeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *lastTradePercentChangeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *lastTradeTimeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *lowLabel;
//@property (nonatomic, retain) IBOutlet UILabel *highLabel;
//@property (nonatomic, retain) IBOutlet UILabel *volumeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *openLabel;
//@property (nonatomic, retain) IBOutlet UILabel *previousCloseLabel;
//@property (nonatomic, retain) IBOutlet UILabel *openChangeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *openPercentChangeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *vwapLabel;
//@property (nonatomic, retain) IBOutlet UILabel *bidPriceLabel;
//@property (nonatomic, retain) IBOutlet UILabel *bidSizeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *askPriceLabel;
//@property (nonatomic, retain) IBOutlet UILabel *askSizeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *countryLabel;
//@property (nonatomic, retain) IBOutlet UILabel *currencyLabel;
//@property (nonatomic, retain) IBOutlet UILabel *outstandingSharesLabel;
//@property (nonatomic, retain) IBOutlet UILabel *marketCapitalizationLabel;
//@property (nonatomic, retain) IBOutlet UILabel *buyLotLabel;
//@property (nonatomic, retain) IBOutlet UILabel *butLotValueLabel;
//@property (nonatomic, retain) IBOutlet UILabel *turnoverLabel;
//@property (nonatomic, retain) IBOutlet UILabel *onVolumeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *onValueLabel;
//@property (nonatomic, retain) IBOutlet UILabel *averageVolumeLabel;
//@property (nonatomic, retain) IBOutlet UILabel *averageValueLabel;
//
//@property (nonatomic, retain) IBOutlet UIButton *chart;
//@property (nonatomic, retain) IBOutlet UIActivityIndicatorView *chartActivity;

- (UIView *)loadViewFromNibNamed:(NSString *)nibName;
//- (IBAction)imageWasTapped:(id)sender;
//- (void)centerSelection:(id)sender;
//- (void)setValues;
//- (void)setChart;

@end

