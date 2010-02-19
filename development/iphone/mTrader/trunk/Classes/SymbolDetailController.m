//
//  SymbolDetailController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolDetailController.h"

#import <QuartzCore/QuartzCore.h>

#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Chart.h"

@implementation SymbolDetailController
@synthesize symbol = _symbol;
@synthesize toolBar;

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
    if (self = [super init]) {
		_symbol = symbol;
		[self.symbol addObserver:self forKeyPath:@"symbol" options:NSKeyValueObservingOptionNew context:nil];
		[self.symbol addObserver:self forKeyPath:@"symbolDynamicData.lastTrade" options:NSKeyValueObservingOptionNew context:nil];
		[self.symbol addObserver:self forKeyPath:@"chart.data" options:NSKeyValueObservingOptionNew context:nil];

		period = 0;
		
		self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	}
    return self;
}

- (void)viewDidLoad {
	dateFormatter = [[NSDateFormatter alloc] init];
	[dateFormatter setDateStyle:NSDateFormatterShortStyle];
	
	timeFormatter = [[NSDateFormatter alloc] init];
	[timeFormatter setDateFormat:@"HH:mm:ss"];
	
	doubleFormatter = [[NSNumberFormatter alloc] init];
	[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	[doubleFormatter setUsesSignificantDigits:YES];
	
	integerFormatter = [[NSNumberFormatter alloc] init];
	[integerFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	
	percentFormatter = [[NSNumberFormatter alloc] init];
	[percentFormatter setNumberStyle:NSNumberFormatterPercentStyle];
	[percentFormatter setUsesSignificantDigits:YES];
	
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] staticDataForFeedTicker:feedTicker];
	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:period width:280 height:280 orientation:@"A"];
	
	headerFont = [UIFont boldSystemFontOfSize:18.0];
	mainFont = [UIFont systemFontOfSize:14.0];
	
	CGRect viewBounds = self.view.bounds;
	scrollView = [[UIScrollView alloc] initWithFrame:viewBounds];
	scrollView.autoresizingMask = UIViewAutoresizingFlexibleHeight;
	[scrollView setBackgroundColor:[UIColor whiteColor]];
	[self.view addSubview:scrollView];
	
	[self setupPage];
	[self updateSymbolInformation];
	[self updateTradesInformation];
	[self updateFundamentalsInformation];
	[self updateChart];
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	//NSLog(@"KVO Update: %@ %@ %@ %@", keyPath, object, change, context);
	if ([keyPath isEqualToString:@"symbol"]) {
		[self updateSymbolInformation];
	} else if ([keyPath isEqualToString:@"symbolDynamicData"]) {
		[self updateTradesInformation];
	} else if ([keyPath isEqualToString:@"chart.data"]) {
		[self updateChart];
	}
}

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

- (UIView *)setHeader:(NSString *)header {
	UIColor *sectionTextColor = [UIColor colorWithWhite:1.0 alpha:1.0];
	UIColor *sectionTextShadowColor = [UIColor colorWithWhite:0.0 alpha:0.44];
	CGSize shadowOffset = CGSizeMake(0.0, 1.0);
	
	// Render the dynamic gradient
	CAGradientLayer *headerGradient = [CAGradientLayer layer];
	UIColor *topLine = [UIColor colorWithRed:111.0/255.0 green:118.0/255.0 blue:123.0/255.0 alpha:1.0];
	UIColor *shine = [UIColor colorWithRed:165.0/255.0 green:177/255.0 blue:186.0/255.0 alpha:1.0];
	UIColor *topOfFade = [UIColor colorWithRed:144.0/255.0 green:159.0/255.0 blue:170.0/255.0 alpha:1.0];
	UIColor *bottomOfFade = [UIColor colorWithRed:184.0/255.0 green:193.0/255.0 blue:200.0/255.0 alpha:1.0];
	UIColor *bottomLine = [UIColor colorWithRed:152.0/255.0 green:158.0/255.0 blue:164.0/255.0 alpha:1.0];
	NSArray *colors = [NSArray arrayWithObjects:(id)topLine.CGColor, (id)shine.CGColor, (id)topOfFade.CGColor, (id)bottomOfFade.CGColor, (id)bottomLine.CGColor, nil];
	NSArray *locations = [NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0], [NSNumber numberWithFloat:0.05],[NSNumber numberWithFloat:0.10],[NSNumber numberWithFloat:0.95],[NSNumber numberWithFloat:1.0],nil];
	headerGradient.colors = colors;
	headerGradient.locations = locations;
	
	CGSize headerSize = [header sizeWithFont:headerFont];
	
	CGRect labelFrame = CGRectMake(TEXT_LEFT_MARGIN, 0.0, headerSize.width, headerSize.height);
	
	UIView *headerView = [[[UIView alloc] initWithFrame:CGRectZero] autorelease];
	UILabel *label = [[UILabel alloc] initWithFrame:labelFrame];
	
	[headerView.layer insertSublayer:headerGradient atIndex:0];
	headerGradient.frame = headerView.bounds;
	
	label.text = header;
	[label setFont:headerFont];
	[label setTextColor:sectionTextColor];
	[label setShadowColor:sectionTextShadowColor];
	[label setShadowOffset:shadowOffset];
	[label setBackgroundColor:[UIColor clearColor]];
	
	[headerView addSubview:label];
	[scrollView addSubview:headerView];
		
	[label release];
	return headerView;
}

- (UILabel *)generateLabelWithFrame:(CGRect)frame {
	UILabel *label = [[[UILabel alloc] initWithFrame:frame] autorelease];
	[label setTextColor:[UIColor blackColor]];
	[label setBackgroundColor:[UIColor clearColor]];
	[label setFont:mainFont];
	[scrollView addSubview:label];
	return label;
}

- (UIButton *)generateButtonWithFrame:(CGRect)frame {
	UIButton *button = [[[UIButton alloc] initWithFrame:frame] autorelease];

	[chartButton addTarget:self action:@selector(imageWasTapped:) forControlEvents:UIControlEventTouchUpInside];
	[scrollView addSubview:chartButton];
	return button;
}

- (CGRect)headerFrame {
	CGSize headerSize = [@"X" sizeWithFont:headerFont];
	return CGRectMake(0.0, globalY, self.view.bounds.size.width, headerSize.height);
}

- (CGRect)leftSideFrame {
	CGSize fontSize = [@"X" sizeWithFont:mainFont];
	return CGRectMake(TEXT_LEFT_MARGIN, globalY, self.view.bounds.size.width / 2 - TEXT_LEFT_MARGIN, fontSize.height);
}

- (CGRect)rightSideFrame {
	CGSize fontSize = [@"X" sizeWithFont:mainFont];
	return CGRectMake(self.view.bounds.size.width / 2, globalY, self.view.bounds.size.width / 2 - TEXT_RIGHT_MARGIN, fontSize.height);
}

- (void)setupPage {
	globalY = 0.0;
	CGRect frame;
	
	NSString *symbolHeader = NSLocalizedString(@"symbolInformationHeader", @"Symbol Information");
	symbolsHeaderView = [[self setHeader:symbolHeader] retain];
	globalY += symbolsHeaderView.bounds.size.height;
	
	tickerName = [[self generateLabelWithFrame:frame] retain];
	[self setFrame:tickerName];
	
	type = [[self generateLabelWithFrame:frame] retain];
	isin = [[self generateLabelWithFrame:frame] retain];
	[self setLeftFrame:type	andRightFrame:isin];
	
	currency = [[self generateLabelWithFrame:frame] retain];
	country = [[self generateLabelWithFrame:frame] retain];
	[self setLeftFrame:currency	andRightFrame:isin];
	
	NSString *tradesHeader = NSLocalizedString(@"tradesInformationHeader", @"Trades Information");
	tradesHeaderView = [[self setHeader:tradesHeader] retain];
	lastTrade = [[self generateLabelWithFrame:frame] retain];
	vwap = [[self generateLabelWithFrame:frame] retain];
	lastTradeTime = [[self generateLabelWithFrame:frame] retain];
	open = [[self generateLabelWithFrame:frame] retain];
	turnover = [[self generateLabelWithFrame:frame] retain];
	high = [[self generateLabelWithFrame:frame] retain];
	volume = [[self generateLabelWithFrame:frame] retain];
	low = [[self generateLabelWithFrame:frame] retain];

	NSString *fundamentalsHeader = NSLocalizedString(@"fundamentalsInformationHeader", @"Fundamentals");
	fundamentalsHeaderView = [[self setHeader:fundamentalsHeader] retain];
	segment = [[self generateLabelWithFrame:frame] retain];
	marketCapitalization = [[self generateLabelWithFrame:frame] retain];
	outstandingShares = [[self generateLabelWithFrame:frame] retain];
	dividend = [[self generateLabelWithFrame:frame] retain];
	dividendDate = [[self generateLabelWithFrame:frame] retain];
	
	NSString *chartHeader = NSLocalizedString(@"chartHeader", @"Chart");
	chartHeaderView = [[self setHeader:chartHeader] retain];
	chartButton = [[self generateButtonWithFrame:frame] retain];
	
	scrollView.contentSize = CGSizeMake(scrollView.bounds.size.width, globalY);
}

- (void)updateSymbolInformation {
	tickerName.text = [NSString stringWithFormat:@"%@ %@", self.symbol.tickerSymbol, self.symbol.companyName];
	type.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"type", @"LocalizedString"), type];
	isin.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"isin", @"LocalizedString"), isin];
	currency.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"currency", @"LocalizedString"), currency];
	country.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"country", @"LocalizedString"), country];
}

- (void)updateTradesInformation {	
	lastTrade.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"lastTrade", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTrade]];
	vwap.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"vwap", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.VWAP]];
	lastTradeTime.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"lastTradeTime", @"LocalizedString"), [timeFormatter stringFromDate:self.symbol.symbolDynamicData.lastTradeTime]];
	open.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"open", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.open]];
	turnover.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"turnover", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.turnover]];
	high.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"high", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.high]];
	volume.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"volume", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.volume]];
	low.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"low", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.low]];
}
- (void)updateFundamentalsInformation {
	segment.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"segment", @"LocalizedString"), self.symbol.symbolDynamicData.segment];
	marketCapitalization.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"marketCapialization", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.marketCapitalization]];
	outstandingShares.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"outstandingShares", @"LocalizedString"), [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.outstandingShares]];
	dividend.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"dividend", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.dividend]];
	dividendDate.text = [NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"dividendDate", @"LocalizedString"), [dateFormatter stringFromDate:self.symbol.symbolDynamicData.dividendDate]];
}

- (void)updateChart {
	UIImage *image = [UIImage imageWithData:self.symbol.chart.data];
	[chartButton setBackgroundImage:image forState:UIControlStateNormal];
}

#pragma mark -
#pragma mark Actions
- (void)imageWasTapped:(id)sender {
	//[chartActivity startAnimating];
	//chartActivity.hidden = NO;
	switch (period) {
		case 0:
			period = 30;
			break;
		case 30:
			period = 365;
			break;
		case 365:
			period = 0;
			break;
		default:
			break;
	}
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:period width:280 height:280 orientation:@"A"];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[self.symbol removeObserver:self forKeyPath:@"symbolDynamicData.lastTrade"];
	[self.symbol removeObserver:self forKeyPath:@"chart.data"];
	
	[tickerName release];
	[type release];
	[isin release];
	[currency release];
	[country release];
	[lastTrade release];
	[vwap release];
	[lastTradeTime release];
	[open release];
	[turnover release];
	[high release];
	[volume release];
	[low release];
	[segment release];
	[marketCapitalization release];
	[outstandingShares release];
	[dividend release];
	[dividendDate release];
	
	[dateFormatter release];
	[timeFormatter release];
	[doubleFormatter release];
	[integerFormatter release];
	[percentFormatter release];
	
	[mainFont release];
	[headerFont release];
	
	[dataView release];
	[chartButton release];
	[scrollView release];
	[_symbol release];

	[super dealloc];
}

@end
