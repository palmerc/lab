//
//  SymbolDetailController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolDetailController.h"
#import "SymbolDetailPageOne.h"
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"

@implementation SymbolDetailController
@synthesize symbol;
@synthesize toolBar;
@synthesize scrollView;
//@synthesize nameLabel;
//@synthesize tickerSymbolLabel;
//@synthesize typeLabel;
//@synthesize statusLabel;
//@synthesize stockISINLabel;
//@synthesize lastTradeLabel;
//@synthesize lastTradeChangeLabel;
//@synthesize lastTradePercentChangeLabel;
//@synthesize lastTradeTimeLabel;
//@synthesize lowLabel;
//@synthesize highLabel;
//@synthesize volumeLabel;
//@synthesize openLabel;
//@synthesize openChangeLabel;
//@synthesize openPercentChangeLabel;
//@synthesize previousCloseLabel;
//@synthesize vwapLabel;
//@synthesize bidPriceLabel;
//@synthesize bidSizeLabel;
//@synthesize askPriceLabel;
//@synthesize askSizeLabel;
//@synthesize countryLabel;
//@synthesize currencyLabel;
//@synthesize outstandingSharesLabel;
//@synthesize marketCapitalizationLabel;
//@synthesize buyLotLabel;
//@synthesize butLotValueLabel;
//@synthesize turnoverLabel;
//@synthesize onVolumeLabel;
//@synthesize onValueLabel;
//@synthesize averageVolumeLabel;
//@synthesize averageValueLabel;
//@synthesize chart;
//@synthesize chartActivity;


- (id)init {
	self = [super initWithNibName:@"SymbolDetailView" bundle:nil];
	if (self != nil) {
		[self.view addSubview:self.scrollView];
		CGSize size;
		size.height = 920;
		size.width = 320;
		self.scrollView.contentSize = size;

		pageOneView = [[SymbolDetailPageOne alloc] init];
		[self.scrollView addSubview:pageOneView];

		UIView *pageTwoView = [self loadViewFromNibNamed:@"SymbolDetailPageTwo"];
		CGRect frameTwo = CGRectMake(0, 461, 320, 460);
		[pageTwoView setFrame:frameTwo];
		[self.scrollView addSubview:pageTwoView];
		//period = 0;
	}
	
	return self;
}

- (UIView *)loadViewFromNibNamed:(NSString *)nibName {
	NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:nibName owner:self options:nil];

	for (id object in topLevelObjects) {
		if ([object isKindOfClass:[UIView class]]) {
			return object;
		}
	}
	return nil;
}

- (void)viewWillAppear:(BOOL)animated {
	pageOneView.symbol = self.symbol;
	[pageOneView renderMe];
}

//- (void)viewWillAppear:(BOOL)animated {
//	NSArray *centerItems = [NSArray arrayWithObjects:@"Detail", @"OrderBook", nil];
//	UISegmentedControl *centerControl = [[UISegmentedControl alloc] initWithItems:centerItems];
//	centerControl.segmentedControlStyle = UISegmentedControlStyleBar;
//	centerControl.selectedSegmentIndex = 0;
//	[centerControl addTarget:self action:@selector(centerSelection:) forControlEvents:UIControlEventValueChanged];
//	
//	UIBarButtonItem *centerBarItem = [[UIBarButtonItem alloc] initWithCustomView:centerControl];
//	[centerControl release];
//	[self.toolBar setItems:[NSArray arrayWithObjects:centerBarItem, nil]];
//	[centerBarItem release];
//	
//}
//
//- (void)viewWillDisappear:(BOOL)animated {
//	self.toolBar.items = nil;
//}
//
//- (void)centerSelection:(id)sender {
//	NSLog(@"%@", sender);
//	
//	
//}

- (void)setValues {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateStyle:NSDateFormatterShortStyle];
	}
	
	static NSDateFormatter *timeFormatter = nil;
	if (timeFormatter == nil) {
		timeFormatter = [[NSDateFormatter alloc] init];
		[timeFormatter setDateFormat:@"HH:mm:ss"];
	}
	
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
		[doubleFormatter setUsesSignificantDigits:YES];
	}
	
	static NSNumberFormatter *integerFormatter = nil;
	if (integerFormatter == nil) {
		integerFormatter = [[NSNumberFormatter alloc] init];
		[integerFormatter setNumberStyle:NSNumberFormatterNoStyle];
	}
	
	static NSNumberFormatter *percentFormatter = nil;
	if (percentFormatter == nil) {
		percentFormatter = [[NSNumberFormatter alloc] init];
		[percentFormatter setNumberStyle:NSNumberFormatterPercentStyle];
		[percentFormatter setUsesSignificantDigits:YES];
	}
	
	//pageOneView.descriptionLabel.text = self.symbol.companyName;
//	pageOneView..text = self.symbol.tickerSymbol;
//	pageOneView.typeLabel.text = self.symbol.type;
//	pageOneView.statusLabel.text = self.symbol.symbolDynamicData.tradingStatus;
//	pageOneView.stockISINLabel.text = self.symbol.isin;
//	pageOneView.lastTradeLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTrade];
//	pageOneView.lastTradeChangeLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTradeChange];
//	pageOneView.lastTradePercentChangeLabel.text = [NSString stringWithFormat:@"%@%%", self.symbol.symbolDynamicData.lastTradePercentChange];
//	NSDate *tradeTime = self.symbol.symbolDynamicData.lastTradeTime;
//	NSString *timeString = [NSString stringWithFormat:@"%@ %@", [dateFormatter stringFromDate:tradeTime], [timeFormatter stringFromDate:tradeTime]];
//	pageOneView.lastTradeTimeLabel.text = timeString;
//	pageOneView.openChangeLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.openChange];
//	pageOneView.openPercentChangeLabel.text = [percentFormatter stringFromNumber:self.symbol.symbolDynamicData.openPercentChange];
//	pageOneView.lowLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.low];
//	pageOneView.highLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.high];
//	pageOneView.volumeLabel.text = [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.volume];
//	pageOneView.openLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.open];
//	pageOneView.previousCloseLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.previousClose];
//	pageOneView.vwapLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.VWAP];
//	pageOneView.bidPriceLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.bidPrice];
//	pageOneView.bidSizeLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.bidSize];
//	pageOneView.askPriceLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.askPrice];
//	pageOneView.askSizeLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.askSize];
//	pageOneView.countryLabel.text = self.symbol.country;
//	pageOneView.currencyLabel.text = self.symbol.currency;
//	pageOneView.outstandingSharesLabel.text = [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.outstandingShares];
//	pageOneView.marketCapitalizationLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.marketCapitalization];
//	pageOneView.buyLotLabel.text = [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.buyLot];
//	pageOneView.butLotValueLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.buyLotValue];
//	pageOneView.turnoverLabel.text = [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.turnover];
//	pageOneView.onVolumeLabel.text = [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.onVolume];
//	pageOneView.onValueLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.onValue];
//	pageOneView.averageVolumeLabel.text = [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.averageVolume];
//	pageOneView.averageValueLabel.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.averageValue];
}

//- (IBAction)imageWasTapped:(id)sender {
//	[chartActivity startAnimating];
//	chartActivity.hidden = NO;
//	switch (period) {
//		case 0:
//			period = 30;
//			break;
//		case 30:
//			period = 365;
//			break;
//		case 365:
//			period = 0;
//			break;
//		default:
//			break;
//	}
//	
//	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
//	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:0 width:280 height:280 orientation:@"A"];
//}

//- (void)staticUpdated:(NSString *)feedTicker {
//	[chartActivity startAnimating];
//	chartActivity.hidden = NO;
//	[self setValues];
//	/*
//	UIImage *image = [UIImage imageWithData:[self.symbol.chart image]];
//	
//	if (image != nil) {
//		chartActivity.hidden = YES;
//		[chartActivity stopAnimating];
//		[self.chart setImage:image forState:UIControlStateNormal];
//	}
//	 */
//}

- (void)dealloc {
	[symbol release];
	[scrollView release];
//	[tickerSymbolLabel release];
//	[nameLabel release];
//	[typeLabel release];
//	[statusLabel release];
//	[stockISINLabel release];
//	[lastTradeLabel release];
//	[lastTradeChangeLabel release];
//	[lastTradePercentChangeLabel release];
//	[lastTradeTimeLabel release];
//	[lowLabel release];
//	[highLabel release];
//	[volumeLabel release];
//	[openLabel release];
//	[previousCloseLabel release];
//	[openChangeLabel release];
//	[openPercentChangeLabel release];
//	[vwapLabel release];
//	[bidPriceLabel release];
//	[bidSizeLabel release];
//	[askPriceLabel release];
//	[askSizeLabel release];
//	[countryLabel release];
//	[currencyLabel release];
//	[outstandingSharesLabel release];
//	[marketCapitalizationLabel release];
//	[buyLotLabel release];
//	[butLotValueLabel release];
//	[turnoverLabel release];
//	[onVolumeLabel release];
//	[onValueLabel release];
//	[averageVolumeLabel release];
//	[averageValueLabel release];
//	
//	[chart release];
//	[chartActivity release];
	
	[super dealloc];
}

@end
