//
//  SymbolDetailController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolDetailController.h"

#import <QuartzCore/QuartzCore.h>

#import "mTraderCommunicator.h"
#import "QFields.h"
#import "StringHelpers.h"

#import "LastChangeView.h"
#import "TradesInfoView.h"
#import "OrderBookView.h"

#import "RoundedRectangle.h"
#import "OrderBookController.h"
#import "TradesController.h"
#import "ChartController.h";
#import "SymbolNewsController.h"
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Chart.h"

@implementation SymbolDetailController
@synthesize managedObjectContext;
@synthesize symbol = _symbol;
@synthesize toolBar;

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
    if (self = [super init]) {
		self.symbol = symbol;
	}
    return self;
}

- (void)viewDidLoad {
	self.toolBar.hidden = YES;
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
	
	CGRect lastFrame = CGRectMake(0.0, 0.0, 160.0, 220.0);
	lastBox = [[LastChangeView alloc] initWithFrame:lastFrame];
	lastBox.symbol = self.symbol;

	CGRect tradesFrame = CGRectMake(160.0, 0.0, 160.0, 220.0);
	tradesBox = [[TradesInfoView alloc] initWithFrame:tradesFrame];
	tradesBox.symbol = self.symbol;
	
	CGRect orderFrame = CGRectMake(0.0, 220.0, 320.0, 150.0);
	orderBox = [[OrderBookView alloc] initWithFrame:orderFrame andManagedObjectContext:self.managedObjectContext];
	orderBox.symbol = self.symbol;
	
	[self.view addSubview:lastBox];
	[self.view addSubview:tradesBox];
	[self.view addSubview:orderBox];
}

- (void)viewWillAppear:(BOOL)animated {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[communicator staticDataForFeedTicker:feedTicker];
	
	QFields *qFields = [[QFields alloc] init];
	qFields.timeStamp = YES;
	qFields.lastTrade = YES;
	qFields.change = YES;
	qFields.changePercent = YES;
	qFields.open = YES;
	qFields.high = YES;
	qFields.low = YES;
	qFields.volume = YES;
	qFields.orderBook = YES;
	
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:feedTicker];	
}

//- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
//	//NSLog(@"KVO Update: %@ %@ %@ %@", keyPath, object, change, context);
//	if ([keyPath isEqualToString:@"currency"]) {
//		[self updateSymbolInformation];
//		[self updateFundamentalsInformation];
//	} else if ([keyPath isEqualToString:@"symbolDynamicData.lastTrade"]) {
//		[self updateTradesInformation];
//	}
//}

#pragma mark -
#pragma mark Layout the views

#define TEXT_LEFT_MARGIN    8.0
#define TEXT_RIGHT_MARGIN   8.0

#define SECTION_HEADER_HEIGHT 22.0
#define LINE_WIDTH          320.0
#define LINE_HEIGHT         22.0
#define HEIGHT_PADDING		4.0;

#define EDITING_INSET       10.0
#define BUTTON_WIDTH        85.0
#define TIME_WIDTH          102.0
#define DESCRIPTION_WIDTH   200.0

//- (UIView *)setHeader:(NSString *)header {
//	UIColor *sectionTextColor = [UIColor colorWithWhite:1.0 alpha:1.0];
//	UIColor *sectionTextShadowColor = [UIColor colorWithWhite:0.0 alpha:0.44];
//	CGSize shadowOffset = CGSizeMake(0.0, 1.0);
//	
//	// Render the dynamic gradient
//	CAGradientLayer *headerGradient = [CAGradientLayer layer];
//	UIColor *topLine = [UIColor colorWithRed:111.0/255.0 green:118.0/255.0 blue:123.0/255.0 alpha:1.0];
//	UIColor *shine = [UIColor colorWithRed:165.0/255.0 green:177/255.0 blue:186.0/255.0 alpha:1.0];
//	UIColor *topOfFade = [UIColor colorWithRed:144.0/255.0 green:159.0/255.0 blue:170.0/255.0 alpha:1.0];
//	UIColor *bottomOfFade = [UIColor colorWithRed:184.0/255.0 green:193.0/255.0 blue:200.0/255.0 alpha:1.0];
//	UIColor *bottomLine = [UIColor colorWithRed:152.0/255.0 green:158.0/255.0 blue:164.0/255.0 alpha:1.0];
//	NSArray *colors = [NSArray arrayWithObjects:(id)topLine.CGColor, (id)shine.CGColor, (id)topOfFade.CGColor, (id)bottomOfFade.CGColor, (id)bottomLine.CGColor, nil];
//	NSArray *locations = [NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0], [NSNumber numberWithFloat:0.05],[NSNumber numberWithFloat:0.10],[NSNumber numberWithFloat:0.95],[NSNumber numberWithFloat:1.0],nil];
//	headerGradient.colors = colors;
//	headerGradient.locations = locations;
//	
//	CGSize headerSize = [header sizeWithFont:headerFont];
//	
//	CGRect labelFrame = CGRectMake(TEXT_LEFT_MARGIN, 0.0, headerSize.width, headerSize.height);
//	
//	CGRect viewFrame = CGRectMake(0.0, globalY, scrollView.bounds.size.width, headerSize.height);
//	UIView *headerView = [[[UIView alloc] initWithFrame:viewFrame] autorelease];
//	UILabel *label = [[UILabel alloc] initWithFrame:labelFrame];
//	globalY += headerView.frame.size.height;
//	
//	[headerView.layer insertSublayer:headerGradient atIndex:0];
//	headerGradient.frame = headerView.bounds;
//	
//	label.text = header;
//	[label setFont:headerFont];
//	[label setTextColor:sectionTextColor];
//	[label setShadowColor:sectionTextShadowColor];
//	[label setShadowOffset:shadowOffset];
//	[label setBackgroundColor:[UIColor clearColor]];
//	
//	[headerView addSubview:label];
//	[scrollView addSubview:headerView];
//		
//	[label release];
//	return headerView;
//}
//
//- (void)orderBook:(id)sender {
//	OrderBookController *orderBookController = [[OrderBookController alloc] initWithSymbol:self.symbol];
//	orderBookController.delegate = self;
//	orderBookController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
//	
//	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:orderBookController];
//	[orderBookController release];
//	
//	[self presentModalViewController:navController animated:YES];
//	[navController release];
//}
//
//- (void)trades:(id)sender {
//	TradesController *tradesController = [[TradesController alloc] initWithSymbol:self.symbol];
//	tradesController.delegate = self;
//	tradesController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
//	
//	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:tradesController];
//	[tradesController release];
//	
//	[self presentModalViewController:navController animated:YES];
//	[navController release];
//}
//
//- (void)chart:(id)sender {
//	ChartController *chartController = [[ChartController alloc] initWithSymbol:self.symbol];
//	chartController.delegate = self;
//	chartController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
//	chartController.managedObjectContext = self.managedObjectContext;
//	
//	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:chartController];
//	[chartController release];
//	
//	[self presentModalViewController:navController animated:YES];
//	[navController release];
//}
//
//- (void)news:(id)sender {
//	SymbolNewsController *symbolNewsController = [[SymbolNewsController alloc] initWithSymbol:self.symbol];
//	symbolNewsController.delegate = self;
//	symbolNewsController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
//	
//	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:symbolNewsController];
//	[symbolNewsController release];
//	
//	[self presentModalViewController:navController animated:YES];
//	[navController release];
//}

//- (UILabel *)generateLabelWithText:(NSString *)text {
//	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectZero] autorelease];
//	[label setText:text];
//	[label setTextColor:[UIColor darkGrayColor]];
//	[label setTextAlignment:UITextAlignmentLeft];
//	[label setBackgroundColor:[UIColor clearColor]];
//	[label setFont:mainFont];
//	[scrollView addSubview:label];
//	return label;
//}
//
//- (UILabel *)generateDataLabel {
//	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectZero] autorelease];
//	[label setTextColor:[UIColor blackColor]];
//	[label setTextAlignment:UITextAlignmentRight];
//	[label setBackgroundColor:[UIColor clearColor]];
//	[label setFont:mainFont];
//	[scrollView addSubview:label];
//	return label;
//}
//
//- (UIButton *)generateButton {
//	UIButton *button = [[[UIButton alloc] initWithFrame:CGRectZero] autorelease];
//
//	[scrollView addSubview:button];
//	return button;
//}
//
//- (CGRect)headerFrame {
//	CGSize headerSize = [@"X" sizeWithFont:headerFont];
//	return CGRectMake(0.0, globalY, self.view.bounds.size.width, headerSize.height);
//}
//
//- (void)setLabelFrame:(UILabel *)label {
//	CGSize fontSize = [@"X" sizeWithFont:mainFont];
//	label.frame = CGRectMake(TEXT_LEFT_MARGIN, globalY, self.view.bounds.size.width - TEXT_LEFT_MARGIN * 2	, fontSize.height);
//	globalY += fontSize.height;
//}
//
//- (void)setLeftLabelFrame:(UILabel *)leftLabel andLeftData:(UILabel *)leftDataLabel andRightLabelFrame:(UILabel *)rightLabel andRightData:(UILabel *)rightDataLabel {
//	CGSize leftFontSize = [leftLabel.text sizeWithFont:mainFontBold];
//	CGFloat halfWidth = self.view.bounds.size.width / 2;
//	leftLabel.frame = CGRectMake(TEXT_LEFT_MARGIN, globalY, leftFontSize.width, leftFontSize.height);
//	leftDataLabel.frame = CGRectMake(TEXT_LEFT_MARGIN + leftFontSize.width, globalY, halfWidth - leftFontSize.width - TEXT_LEFT_MARGIN - 4.0f, leftFontSize.height);
//
//	CGSize rightFontSize = [rightLabel.text sizeWithFont:mainFontBold];
//	rightLabel.frame = CGRectMake(halfWidth + 4.0f, globalY, rightFontSize.width, rightFontSize.height);
//	rightDataLabel.frame = CGRectMake(halfWidth + 4.0f + rightFontSize.width, globalY, self.view.bounds.size.width - rightFontSize.width - 4.0f - halfWidth - TEXT_RIGHT_MARGIN, rightFontSize.height);
//
//	globalY += leftFontSize.height;
//}
//
//- (void)setupPage {
//	globalY = 0.0;
//	
//	NSString *symbolHeader = NSLocalizedString(@"symbolInformationHeader", @"Symbol Information");
//	symbolsHeaderView = [[self setHeader:symbolHeader] retain];
//	
//	tickerName = [[self generateLabelWithText:nil] retain];
//	tickerName.textColor = [UIColor blackColor];
//	tickerName.font = mainFontBold;
//	
//	[self setLabelFrame:tickerName];
//	
//	NSString *typeLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"type", @"LocalizedString")];
//	typeLabel = [[self generateLabelWithText:typeLabelString] retain];
//	type = [[self generateDataLabel] retain];
//	
//	NSString *isinLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"isin", @"LocalizedString")];
//	isinLabel = [[self generateLabelWithText:isinLabelString] retain];
//	isin = [[self generateDataLabel] retain];
//	
//	[self setLeftLabelFrame:typeLabel andLeftData:type andRightLabelFrame:isinLabel andRightData:isin];
//	
//	NSString *currencyLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"currency", @"LocalizedString")];
//	currencyLabel = [[self generateLabelWithText:currencyLabelString] retain];
//	currency = [[self generateDataLabel] retain];
//	NSString *countryLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"country", @"LocalizedString")];
//	countryLabel = [[self generateLabelWithText:countryLabelString] retain];
//	country = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:currencyLabel andLeftData:currency andRightLabelFrame:countryLabel andRightData:country];
//	
//	NSString *tradesHeader = NSLocalizedString(@"tradesInformationHeader", @"Trades Information");
//	tradesHeaderView = [[self setHeader:tradesHeader] retain];
//	
//	NSString *lastTradeLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"lastTrade", @"LocalizedString")];
//	lastTradeLabel = [[self generateLabelWithText:lastTradeLabelString] retain];
//	
//	lastTrade = [[self generateDataLabel] retain];
//	NSString *openLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"open", @"LocalizedString")];
//	openLabel = [[self generateLabelWithText:openLabelString] retain];
//	open = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:lastTradeLabel andLeftData:lastTrade andRightLabelFrame:openLabel andRightData:open];
//
//	NSString *lastTradeTimeLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"lastTradeTime", @"LocalizedString")];
//	lastTradeTimeLabel = [[self generateLabelWithText:lastTradeTimeLabelString] retain];
//	lastTradeTime = [[self generateDataLabel] retain];
//	NSString *highLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"high", @"LocalizedString")];
//	highLabel = [[self generateLabelWithText:highLabelString] retain];
//	high = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:lastTradeTimeLabel andLeftData:lastTradeTime andRightLabelFrame:highLabel andRightData:high];
//	
//	NSString *lastTradeChangeLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"lastTradeChange", @"LocalizedString")];
//	lastTradeChangeLabel = [[self generateLabelWithText:lastTradeChangeLabelString] retain];
//	lastTradeChange = [[self generateDataLabel] retain];
//	NSString *lowLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"low", @"LocalizedString")];
//	lowLabel = [[self generateLabelWithText:lowLabelString] retain];
//	low = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:lastTradeChangeLabel andLeftData:lastTradeChange andRightLabelFrame:lowLabel andRightData:low];
//
//	NSString *lastTradePercentChangeLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"lastTradePercentChange", @"LocalizedString")];
//	lastTradePercentChangeLabel = [[self generateLabelWithText:lastTradePercentChangeLabelString] retain];
//	lastTradePercentChange = [[self generateDataLabel] retain];
//	NSString *buyLotLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"buyLot", @"LocalizedString")];
//	buyLotLabel = [[self generateLabelWithText:buyLotLabelString] retain];
//	buyLot = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:lastTradePercentChangeLabel andLeftData:lastTradePercentChange andRightLabelFrame:buyLotLabel andRightData:buyLot];
//
//	NSString *vwapLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"vwap", @"LocalizedString")];
//	vwapLabel = [[self generateLabelWithText:vwapLabelString] retain];
//	vwap = [[self generateDataLabel] retain];
//	NSString *buyLotValueLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"buyLotValue", @"LocalizedString")];
//	buyLotValueLabel = [[self generateLabelWithText:buyLotValueLabelString] retain];
//	buyLotValue = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:vwapLabel andLeftData:vwap andRightLabelFrame:buyLotValueLabel andRightData:buyLotValue];
//
//	NSString *tradesLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"trades", @"LocalizedString")];
//	tradesLabel = [[self generateLabelWithText:tradesLabelString] retain];
//	trades = [[self generateDataLabel] retain];
//	NSString *averageVolumeLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"averageVolume", @"LocalizedString")];
//	averageVolumeLabel = [[self generateLabelWithText:averageVolumeLabelString] retain];
//	averageVolume = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:tradesLabel andLeftData:trades andRightLabelFrame:averageVolumeLabel andRightData:averageVolume];
//
//	NSString *turnoverLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"turnover", @"LocalizedString")];
//	turnoverLabel = [[self generateLabelWithText:turnoverLabelString] retain];
//	turnover = [[self generateDataLabel] retain];
//	NSString *averageValueLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"averageValue", @"LocalizedString")];
//	averageValueLabel = [[self generateLabelWithText:averageValueLabelString] retain];
//	averageValue = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:turnoverLabel andLeftData:turnover andRightLabelFrame:averageValueLabel andRightData:averageValue];
//
//	NSString *volumeLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"volume", @"LocalizedString")];
//	volumeLabel = [[self generateLabelWithText:volumeLabelString] retain];
//	volume = [[self generateDataLabel] retain];
//	NSString *onVolumeLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"onVolume", @"LocalizedString")];
//	onVolumeLabel = [[self generateLabelWithText:onVolumeLabelString] retain];
//	onVolume = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:volumeLabel andLeftData:volume andRightLabelFrame:onVolumeLabel andRightData:onVolume];
//
//	NSString *tradingStatusLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"tradingStatus", @"LocalizedString")];
//	tradingStatusLabel = [[self generateLabelWithText:tradingStatusLabelString] retain];
//	tradingStatus = [[self generateDataLabel] retain];	
//	[self setLabelFrame:tradingStatus];
//
//	NSString *fundamentalsHeader = NSLocalizedString(@"fundamentalsInformationHeader", @"Fundamentals");
//	fundamentalsHeaderView = [[self setHeader:fundamentalsHeader] retain];
//	
//	NSString *segmentLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"segment", @"LocalizedString")];
//	segment = [[self generateLabelWithText:segmentLabelString] retain];
//	segment.font = mainFont;
//	[self setLabelFrame:segment];
//	
//	NSString *marketCapitalizationLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"marketCapitalization", @"LocalizedString")];
//	marketCapitalizationLabel = [[self generateLabelWithText:marketCapitalizationLabelString] retain];
//	marketCapitalization = [[self generateDataLabel] retain];
//	NSString *outstandingSharesLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"outstandingShares", @"LocalizedString")];
//	outstandingSharesLabel = [[self generateLabelWithText:outstandingSharesLabelString] retain];outstandingShares = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:marketCapitalizationLabel andLeftData:marketCapitalization andRightLabelFrame:outstandingSharesLabel andRightData:outstandingShares];
//	
//	NSString *dividendLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"dividend", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.dividend]];
//	dividendLabel = [[self generateLabelWithText:dividendLabelString] retain];
//	dividend = [[self generateDataLabel] retain];
//	NSString *dividendDateLabelString = [NSString stringWithFormat:@"%@:", NSLocalizedString(@"dividendDate", @"LocalizedString"), [dateFormatter stringFromDate:self.symbol.symbolDynamicData.dividendDate]];
//	dividendDateLabel = [[self generateLabelWithText:dividendDateLabelString] retain];
//	dividendDate = [[self generateDataLabel] retain];
//	[self setLeftLabelFrame:dividendLabel andLeftData:dividend andRightLabelFrame:dividendDateLabel andRightData:dividendDate];	
//	
//	scrollView.contentSize = CGSizeMake(scrollView.bounds.size.width, globalY);
//}
//
//- (void)updateSymbolInformation {
//	tickerName.text = [NSString stringWithFormat:@"%@", self.symbol.companyName];
//	type.text = [NSString stringWithFormat:@"%@", self.symbol.type];
//	isin.text = [NSString stringWithFormat:@"%@", self.symbol.isin];
//	currency.text = [NSString stringWithFormat:@"%@", self.symbol.currency];
//	country.text = [NSString stringWithFormat:@"%@", self.symbol.country];
//}
//
//- (void)updateTradesInformation {	
//	lastTrade.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTrade]];
//	lastTradeTime.text = [NSString stringWithFormat:@"%@", [timeFormatter stringFromDate:self.symbol.symbolDynamicData.lastTradeTime]];
//	lastTradeChange.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTradeChange]];
//	lastTradePercentChange.text = [NSString stringWithFormat:@"%@", [percentFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTradePercentChange]];
//	vwap.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.VWAP]];
//	open.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.open]];
//	
//	turnover.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.turnover]];
//	high.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.high]];
//	
//	volume.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.volume]];
//	low.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.low]];
//	buyLot.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.buyLot]];
//	buyLotValue.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.buyLotValue]];
//	
//	trades.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.onVolume]];
//	tradingStatus.text = [NSString stringWithFormat:@"%@", self.symbol.symbolDynamicData.tradingStatus];
//	averageValue.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.averageValue]];
//	averageVolume.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.averageVolume]];
//	
//	onVolume.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.onVolume]];
//}
//
//- (void)updateFundamentalsInformation {
//	segment.text = [NSString stringWithFormat:@"%@", self.symbol.symbolDynamicData.segment];
//	
//	marketCapitalization.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.marketCapitalization]];
//	
//	outstandingShares.text = [NSString stringWithFormat:@"%@", [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.outstandingShares]];
//	dividend.text = [NSString stringWithFormat:@"%@", [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.dividend]];
//	dividendDate.text = [NSString stringWithFormat:@"%@", [dateFormatter stringFromDate:self.symbol.symbolDynamicData.dividendDate]];
//}
//
//- (void)orderBookControllerDidFinish:(OrderBookController *)controller {
//	[self dismissModalViewControllerAnimated:YES];
//}
//
//- (void)tradesControllerDidFinish:(TradesController *)controller {
//	[self dismissModalViewControllerAnimated:YES];
//}
//
//- (void)chartControllerDidFinish:(ChartController *)controller {
//	[self dismissModalViewControllerAnimated:YES];
//}
//
//- (void)symbolNewsControllerDidFinish:(SymbolNewsController *)controller {
//	[self dismissModalViewControllerAnimated:YES];
//}
//

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in SymbolDetailController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	//[self.symbol removeObserver:self forKeyPath:@"symbolDynamicData.lastTrade"];
	
//	[tickerName release];
//	[type release];
//	[isin release];
//	[currency release];
//	[country release];
//	[lastTrade release];
//	[vwap release];
//	[lastTradeTime release];
//	[open release];
//	[turnover release];
//	[high release];
//	[volume release];
//	[low release];
//	[segment release];
//	[marketCapitalization release];
//	[outstandingShares release];
//	[dividend release];
//	[dividendDate release];
//	[lastTradeChange release];
//	[lastTradePercentChange release];
//	[buyLot release];
//	[buyLotValue release];
//	[trades release];
//	[tradingStatus release];
//	[averageValue release];
//	[averageVolume release];
//	[onVolume release];
//	[scrollView release];
	[lastBox release];
	[tradesBox release];
	
	[_symbol release];
	[managedObjectContext release];
	[super dealloc];
}

@end
