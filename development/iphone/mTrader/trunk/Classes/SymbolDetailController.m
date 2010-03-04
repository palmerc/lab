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
#import "StringHelpers.h"
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
		[self.symbol addObserver:self forKeyPath:@"currency" options:NSKeyValueObservingOptionNew context:nil];
		[self.symbol addObserver:self forKeyPath:@"symbolDynamicData.lastTrade" options:NSKeyValueObservingOptionNew context:nil];

		globalY = 0.0;
		
		headerFont = [[UIFont boldSystemFontOfSize:18.0] retain];
		mainFont = [[UIFont systemFontOfSize:14.0] retain];		
	}
    return self;
}

- (void)viewDidLoad {
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];

	NSMutableArray *barButtonItems = [[NSMutableArray alloc] init];
	NSString *type = self.symbol.type;
	if (![type isEqualToString:@"Index"] && ![type isEqualToString:@"Exchange Rate"]) {
		UIBarButtonItem *orderBookButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"orderBook", @"Symbol OrderBook") style:UIBarButtonItemStyleBordered target:self action:@selector(orderBook:)];
		[barButtonItems addObject:orderBookButton];
		[orderBookButton release];
	}
	
	UIBarButtonItem *tradesButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"trades", @"Symbol Trades") style:UIBarButtonItemStyleBordered target:self action:@selector(trades:)];
	[barButtonItems addObject:tradesButton];
	[tradesButton release];
	
	UIBarButtonItem *chartButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"chart", @"Symbol Chart") style:UIBarButtonItemStyleBordered target:self action:@selector(chart:)];
	[barButtonItems addObject:chartButton];
	[chartButton release];
	
	if (![type isEqualToString:@"Index"] && ![type isEqualToString:@"Exchange Rate"]) {
		UIBarButtonItem *newsButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"news", @"Symbol News") style:UIBarButtonItemStyleBordered target:self action:@selector(news:)];
		[barButtonItems addObject:newsButton];
		[newsButton release];
	}
	
	[self.toolBar setItems:barButtonItems];
	[barButtonItems release];
	
	
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
	
	CGRect viewBounds = self.view.bounds;
	scrollView = [[UIScrollView alloc] initWithFrame:viewBounds];
	scrollView.autoresizingMask = UIViewAutoresizingFlexibleHeight;
	[scrollView setBackgroundColor:[UIColor whiteColor]];
	[self.view addSubview:scrollView];
	
	[self setupPage];
	[self updateSymbolInformation];
	[self updateTradesInformation];
	[self updateFundamentalsInformation];
}

- (void)viewWillAppear:(BOOL)animated {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	communicator.symbolsDelegate = self;
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[communicator staticDataForFeedTicker:feedTicker];
	[communicator dynamicDetailForFeedTicker:feedTicker];
}

- (void)viewWillDisappear:(BOOL)animated {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	[communicator stopStreamingData];
	communicator.symbolsDelegate = nil;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	//NSLog(@"KVO Update: %@ %@ %@ %@", keyPath, object, change, context);
	if ([keyPath isEqualToString:@"currency"]) {
		[self updateSymbolInformation];
		[self updateFundamentalsInformation];
	} else if ([keyPath isEqualToString:@"symbolDynamicData.lastTrade"]) {
		[self updateTradesInformation];
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
	
	CGRect viewFrame = CGRectMake(0.0, globalY, scrollView.bounds.size.width, headerSize.height);
	UIView *headerView = [[[UIView alloc] initWithFrame:viewFrame] autorelease];
	UILabel *label = [[UILabel alloc] initWithFrame:labelFrame];
	globalY += headerView.frame.size.height;
	
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

- (void)orderBook:(id)sender {
	OrderBookController *orderBookController = [[OrderBookController alloc] initWithSymbol:self.symbol];
	orderBookController.delegate = self;
	orderBookController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:orderBookController];
	[orderBookController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)trades:(id)sender {
	TradesController *tradesController = [[TradesController alloc] initWithSymbol:self.symbol];
	tradesController.delegate = self;
	tradesController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:tradesController];
	[tradesController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)chart:(id)sender {
	ChartController *chartController = [[ChartController alloc] initWithSymbol:self.symbol];
	chartController.delegate = self;
	chartController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	chartController.managedObjectContext = self.managedObjectContext;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:chartController];
	[chartController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)news:(id)sender {
	SymbolNewsController *symbolNewsController = [[SymbolNewsController alloc] initWithSymbol:self.symbol];
	symbolNewsController.delegate = self;
	symbolNewsController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:symbolNewsController];
	[symbolNewsController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (UILabel *)generateLabel {
	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectZero] autorelease];
	[label setTextColor:[UIColor blackColor]];
	[label setBackgroundColor:[UIColor clearColor]];
	[label setFont:mainFont];
	[scrollView addSubview:label];
	return label;
}

- (UIButton *)generateButton {
	UIButton *button = [[[UIButton alloc] initWithFrame:CGRectZero] autorelease];

	[scrollView addSubview:button];
	return button;
}

- (CGRect)headerFrame {
	CGSize headerSize = [@"X" sizeWithFont:headerFont];
	return CGRectMake(0.0, globalY, self.view.bounds.size.width, headerSize.height);
}

- (void)setLabelFrame:(UILabel *)label {
	CGSize fontSize = [@"X" sizeWithFont:mainFont];
	label.frame = CGRectMake(TEXT_LEFT_MARGIN, globalY, self.view.bounds.size.width - TEXT_LEFT_MARGIN * 2	, fontSize.height);
	globalY += fontSize.height;
}

- (void)setLeftLabelFrame:(UILabel *)leftLabel andRightLabelFrame:(UILabel *)rightLabel {
	CGSize fontSize = [@"X" sizeWithFont:mainFont];
	leftLabel.frame = CGRectMake(TEXT_LEFT_MARGIN, globalY, self.view.bounds.size.width / 2 - TEXT_LEFT_MARGIN, fontSize.height);
	rightLabel.frame = CGRectMake(self.view.bounds.size.width / 2, globalY, self.view.bounds.size.width / 2 - TEXT_RIGHT_MARGIN, fontSize.height);
	globalY += fontSize.height;
}

- (void)setupPage {
	globalY = 0.0;
	
	NSString *symbolHeader = NSLocalizedString(@"symbolInformationHeader", @"Symbol Information");
	symbolsHeaderView = [[self setHeader:symbolHeader] retain];
	
	tickerName = [[self generateLabel] retain];
	[self setLabelFrame:tickerName];
	
	type = [[self generateLabel] retain];
	isin = [[self generateLabel] retain];
	[self setLeftLabelFrame:type andRightLabelFrame:isin];
	
	currency = [[self generateLabel] retain];
	country = [[self generateLabel] retain];
	[self setLeftLabelFrame:currency andRightLabelFrame:country];
	
	NSString *tradesHeader = NSLocalizedString(@"tradesInformationHeader", @"Trades Information");
	tradesHeaderView = [[self setHeader:tradesHeader] retain];
	
	lastTrade = [[self generateLabel] retain];
	open = [[self generateLabel] retain];
	[self setLeftLabelFrame:lastTrade andRightLabelFrame:open];

	lastTradeTime = [[self generateLabel] retain];
	high = [[self generateLabel] retain];
	[self setLeftLabelFrame:lastTradeTime andRightLabelFrame:high];
	
	lastTradeChange = [[self generateLabel] retain];
	low = [[self generateLabel] retain];
	[self setLeftLabelFrame:lastTradeChange andRightLabelFrame:low];

	lastTradePercentChange = [[self generateLabel] retain];
	buyLot = [[self generateLabel] retain];
	[self setLeftLabelFrame:lastTradePercentChange andRightLabelFrame:buyLot];

	vwap = [[self generateLabel] retain];
	buyLotValue = [[self generateLabel] retain];
	[self setLeftLabelFrame:vwap andRightLabelFrame:buyLotValue];

	trades = [[self generateLabel] retain];
	averageVolume = [[self generateLabel] retain];
	[self setLeftLabelFrame:trades andRightLabelFrame:averageVolume];

	turnover = [[self generateLabel] retain];
	averageValue = [[self generateLabel] retain];
	[self setLeftLabelFrame:turnover andRightLabelFrame:averageValue];

	volume = [[self generateLabel] retain];
	onVolume = [[self generateLabel] retain];
	[self setLeftLabelFrame:volume andRightLabelFrame:onVolume];

	tradingStatus = [[self generateLabel] retain];	
	[self setLabelFrame:tradingStatus];

	NSString *fundamentalsHeader = NSLocalizedString(@"fundamentalsInformationHeader", @"Fundamentals");
	fundamentalsHeaderView = [[self setHeader:fundamentalsHeader] retain];
	
	segment = [[self generateLabel] retain];
	[self setLabelFrame:segment];
	
	marketCapitalization = [[self generateLabel] retain];
	outstandingShares = [[self generateLabel] retain];
	[self setLeftLabelFrame:marketCapitalization andRightLabelFrame:outstandingShares];
	
	dividend = [[self generateLabel] retain];
	dividendDate = [[self generateLabel] retain];
	[self setLeftLabelFrame:dividend andRightLabelFrame:dividendDate];	
	
	scrollView.contentSize = CGSizeMake(scrollView.bounds.size.width, globalY);
}

- (void)updateSymbolInformation {
	tickerName.text = [NSString stringWithFormat:@"%@", self.symbol.companyName];
	type.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"type", @"LocalizedString"), self.symbol.type];
	isin.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"isin", @"LocalizedString"), self.symbol.isin];
	currency.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"currency", @"LocalizedString"), self.symbol.currency];
	country.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"country", @"LocalizedString"), self.symbol.country];
}

- (void)updateTradesInformation {	
	lastTrade.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"lastTrade", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTrade]];
	lastTradeTime.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"lastTradeTime", @"LocalizedString"), [timeFormatter stringFromDate:self.symbol.symbolDynamicData.lastTradeTime]];
	lastTradeChange.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"lastTradeChange", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTradeChange]];
	lastTradePercentChange.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"lastTradePercentChange", @"LocalizedString"), [percentFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTradePercentChange]];
	vwap.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"vwap", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.VWAP]];
	open.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"open", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.open]];
	turnover.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"turnover", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.turnover]];
	high.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"high", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.high]];
	volume.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"volume", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.volume]];
	low.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"low", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.low]];
	buyLot.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"buyLot", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.buyLot]];
	buyLotValue.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"buyLotValue", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.buyLotValue]];
	trades.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"trades", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.onVolume]];
	tradingStatus.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"tradingStatus", @"LocalizedString"), self.symbol.symbolDynamicData.tradingStatus];
	averageValue.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"averageValue", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.averageValue]];
	averageVolume.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"averageVolume", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.averageVolume]];
	onVolume.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"onVolume", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.onVolume]];
}

- (void)updateFundamentalsInformation {
	segment.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"segment", @"LocalizedString"), self.symbol.symbolDynamicData.segment];
	marketCapitalization.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"marketCapitalization", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.marketCapitalization]];
	outstandingShares.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"outstandingShares", @"LocalizedString"), [integerFormatter stringFromNumber:self.symbol.symbolDynamicData.outstandingShares]];
	dividend.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"dividend", @"LocalizedString"), [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.dividend]];
	dividendDate.text = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"dividendDate", @"LocalizedString"), [dateFormatter stringFromDate:self.symbol.symbolDynamicData.dividendDate]];
}

// l;h;lo;o;v
- (void)updateSymbols:(NSArray *)updates {
	static NSInteger FEED_TICKER = 0;
	static NSInteger LAST_TRADE = 1;
	static NSInteger PERCENT_CHANGE = 2;
	static NSInteger CHANGE = 3;
	static NSInteger HIGH = 4; 
	static NSInteger LOW = 5; 
	static NSInteger OPEN = 6;
	static NSInteger VOLUME = 7;

	/*
	static NSInteger BID_PRICE = 3;
	static NSInteger ASK_PRICE = 4;
	static NSInteger ASK_VOLUME = 5;
	static NSInteger BID_VOLUME = 6;
	*/
	for (NSString *update in updates) {
		
		NSArray *values = [StringHelpers cleanComponents:[update componentsSeparatedByString:@";"]];
		
		NSString *feedTicker = [values objectAtIndex:FEED_TICKER];
		NSArray *feedTickerComponents = [feedTicker componentsSeparatedByString:@"/"];
		NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
		NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
		
		NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
		NSEntityDescription *entity = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
		[request setEntity:entity];
		
		NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed.feedNumber=%@) AND (tickerSymbol=%@)", feedNumber, tickerSymbol];
		[request setPredicate:predicate];
		
		NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
		[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
		[sortDescriptor release];
		
		NSError *error = nil;
		NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
		if (array == nil)
		{
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		}
		
		Symbol *symbol = [array objectAtIndex:0];
		SymbolDynamicData *symbolDynamicData = symbol.symbolDynamicData;
		
		// last trade
		if ([values count] > LAST_TRADE) {
			NSString *lastTradeString = [values objectAtIndex:LAST_TRADE];
			if ([lastTradeString isEqualToString:@"--"] == YES || [lastTradeString isEqualToString:@"-"] == YES) {
				symbolDynamicData.lastTrade = nil;
			} else if ([lastTradeString isEqualToString:@""] == NO) {
				symbolDynamicData.lastTrade = [NSNumber numberWithDouble:[lastTradeString doubleValue]];
				symbolDynamicData.lastTradeTime = [NSDate date];
			}
		}
		// percent change
		if ([values count] > PERCENT_CHANGE) {
			NSString *percentChange = [values objectAtIndex:PERCENT_CHANGE];
			if ([percentChange isEqualToString:@"--"] == YES || [percentChange isEqualToString:@"-"] == YES) {
				symbolDynamicData.changePercent = nil;
			} else if ([percentChange isEqualToString:@""] == NO) {
				symbolDynamicData.changePercent = [NSNumber numberWithDouble:([percentChange doubleValue]/100.0)];
			}
		}
		
		/*
		// bid price
		if ([values count] > BID_PRICE) {
			NSString *bidPrice = [values objectAtIndex:BID_PRICE];
			if ([bidPrice isEqualToString:@"--"] == YES || [bidPrice isEqualToString:@"-"] == YES) {
				symbolDynamicData.bidPrice = nil;
			} else if ([bidPrice isEqualToString:@""] == NO) {
				symbolDynamicData.bidPrice = [NSNumber numberWithDouble:[bidPrice doubleValue]];
			}
		}
		
		// ask price
		if ([values count] > ASK_PRICE) {
			NSString *askPrice = [values objectAtIndex:ASK_PRICE];
			if ([askPrice isEqualToString:@"--"] == YES || [askPrice isEqualToString:@"-"] == YES) {
				symbolDynamicData.askPrice = nil;
			} else if ([askPrice isEqualToString:@""] == NO) {
				symbolDynamicData.askPrice = [NSNumber numberWithDouble:[askPrice doubleValue]];
			}
		}
		
		 // ask volume
		 if ([values count] > ASK_VOLUME) {
		 NSString *askVolume = [values objectAtIndex:ASK_VOLUME];
		 if ([askVolume isEqualToString:@"--"] == YES || [askVolume isEqualToString:@"-"] == YES) {
		 symbolDynamicData.askVolume = nil;
		 } else if ([askVolume isEqualToString:@""] == NO) {
		 NSUInteger multiplier = 1;
		 if ([askVolume rangeOfString:@"k"].location != NSNotFound) {
		 multiplier = 1000;
		 } else if ([askVolume rangeOfString:@"m"].location != NSNotFound) {
		 multiplier = 1000000;
		 }				
		 symbolDynamicData.askVolume = [NSNumber numberWithInteger:[askVolume integerValue] * multiplier];
		 }
		 }
		 
		 // bid volume
		 if ([values count] > BID_VOLUME) {
		 NSString *bidVolume = [values objectAtIndex:BID_VOLUME];
		 if ([bidVolume isEqualToString:@"--"] == YES || [bidVolume isEqualToString:@"-"] == YES) {
		 symbolDynamicData.bidVolume = nil;
		 } else if ([bidVolume isEqualToString:@""] == NO) {
		 NSUInteger multiplier = 1;
		 if ([bidVolume rangeOfString:@"k"].location != NSNotFound) {
		 multiplier = 1000;
		 } else if ([bidVolume rangeOfString:@"m"].location != NSNotFound) {
		 multiplier = 1000000;
		 }				
		 symbolDynamicData.bidVolume = [NSNumber numberWithInteger:[bidVolume integerValue] * multiplier];
		 }
		 }
		*/
		// change
		if ([values count] > CHANGE) {
			NSString *change = [values objectAtIndex:CHANGE];
			if ([change isEqualToString:@"--"] == YES || [change isEqualToString:@"-"] == YES) {
				symbolDynamicData.change = nil;
			} else if ([change isEqualToString:@""] == NO) {
				symbolDynamicData.change = [NSNumber numberWithDouble:[change doubleValue]];
			}
		}
		 
		 // high
		if ([values count] > HIGH) {
			NSString *highString = [values objectAtIndex:HIGH];
			if ([highString isEqualToString:@"--"] == YES || [highString isEqualToString:@"-"] == YES) {
				symbolDynamicData.high = nil;
			} else if ([highString isEqualToString:@""] == NO) {
				symbolDynamicData.high = [NSNumber numberWithDouble:[highString doubleValue]];
			}
		}
		
		// low
		if ([values count] > LOW) {
			NSString *lowString = [values objectAtIndex:LOW];
			if ([lowString isEqualToString:@"--"] == YES || [lowString isEqualToString:@"-"] == YES) {
				symbolDynamicData.low = nil;
			} else if ([lowString isEqualToString:@""] == NO) {
				symbolDynamicData.low = [NSNumber numberWithDouble:[lowString doubleValue]];
			}
		}
		
		// open
		if ([values count] > OPEN) {
			NSString *openString = [values objectAtIndex:OPEN];
			if ([openString isEqualToString:@"--"] == YES || [openString isEqualToString:@"-"] == YES) {
				symbolDynamicData.open = nil;
			} else if ([openString isEqualToString:@""] == NO) {
				symbolDynamicData.open = [NSNumber numberWithDouble:[openString doubleValue]];
			}
		}
		
		// volume
		if ([values count] > VOLUME) {
			NSString *volumeString = [values objectAtIndex:VOLUME];
			if ([volumeString isEqualToString:@"--"] == YES || [volumeString isEqualToString:@"-"] == YES) {
				symbolDynamicData.volume = nil;
			} else if ([volumeString isEqualToString:@""] == NO) {
				float multiplier = 1.0;
				if ([volumeString rangeOfString:@"k"].location != NSNotFound) {
					multiplier = 1000.0;
				} else if ([volumeString rangeOfString:@"m"].location != NSNotFound) {
					multiplier = 1000000.0;
				}
				symbolDynamicData.volume = [NSNumber numberWithDouble:[volumeString doubleValue]  * multiplier];
			}
		 }
		
		array = nil;
	}
	NSError *error;
	if (![self.managedObjectContext save:&error]) {
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
	}
}

- (void)staticUpdates:(NSDictionary *)updateDictionary {
	NSArray *feedTickerComponents = [[updateDictionary objectForKey:@"feedTicker"] componentsSeparatedByString:@"/"];
	NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	
	Symbol *symbol = [self fetchSymbol:tickerSymbol withFeedNumber:feedNumber];
	if ([updateDictionary objectForKey:@"Bid"]) {
		symbol.symbolDynamicData.bidPrice = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Bid"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"B Size"]) {
		NSString *bidSize = [updateDictionary objectForKey:@"B Size"];
		NSUInteger multiplier = 1;
		if ([bidSize rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000;
		} else if ([bidSize rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000;
		}		
		symbol.symbolDynamicData.bidSize = [NSNumber numberWithInteger:[bidSize integerValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"Ask"]) { 
		symbol.symbolDynamicData.askPrice = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Ask"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"A Size"]) { 
		NSString *askSize = [updateDictionary objectForKey:@"A Size"];
		NSUInteger multiplier = 1;
		if ([askSize rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000;
		} else if ([askSize rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000;
		}
		symbol.symbolDynamicData.askSize = [NSNumber numberWithInteger:[askSize integerValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"Pr Cls"]) { 
		symbol.symbolDynamicData.previousClose = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Pr Cls"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"Open"]) { 
		symbol.symbolDynamicData.open = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Open"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"High"]) { 
		symbol.symbolDynamicData.high = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"High"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"Low"]) { 
		symbol.symbolDynamicData.low = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Low"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"Last"]) { 
		symbol.symbolDynamicData.lastTrade = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"Last"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"L +/-"]) { 
		symbol.symbolDynamicData.lastTradeChange = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"L +/-"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"L +/-%"]) {
		symbol.symbolDynamicData.lastTradePercentChange = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"L +/-%"] doubleValue]/100.0f];
	}
	if ([updateDictionary objectForKey:@"O +/-"]) { 
		symbol.symbolDynamicData.openChange = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"O +/-"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"O +/-%"]) {
		symbol.symbolDynamicData.openPercentChange = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"O +/-%"] doubleValue]/100.0f];
	}
	if ([updateDictionary objectForKey:@"Volume"]) {
		NSString *volumeString = [updateDictionary objectForKey:@"Volume"];
		float multiplier = 1.0;
		if ([volumeString rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000.0;
		} else if ([volumeString rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000.0;
		}
		symbol.symbolDynamicData.volume = [NSNumber numberWithDouble:[volumeString doubleValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"Turnover"]) {
		NSString *turnoverString = [updateDictionary objectForKey:@"Turnover"];
		NSUInteger multiplier = 1;
		if ([turnoverString rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000;
		} else if ([turnoverString rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000;
		}		
		symbol.symbolDynamicData.turnover = [NSNumber numberWithInteger:[turnoverString integerValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"OnVolume"]) {
		NSString *onVolumeString = [updateDictionary objectForKey:@"OnVolume"];
		NSUInteger multiplier = 1;
		if ([onVolumeString rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000;
		} else if ([onVolumeString rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000;
		}
		symbol.symbolDynamicData.onVolume = [NSNumber numberWithInteger:[onVolumeString integerValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"OnValue"]) {
		NSString *onValue = [updateDictionary objectForKey:@"OnValue"];
		NSUInteger multiplier = 1;
		if ([onValue rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000;
		} else if ([onValue rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000;
		}
		symbol.symbolDynamicData.onValue = [NSNumber numberWithDouble:[onValue doubleValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"Time"]) { 
		symbol.symbolDynamicData.lastTradeTime = [dateFormatter dateFromString:[updateDictionary objectForKey:@"Time"]];
	}
	if ([updateDictionary objectForKey:@"VWAP"]) { 
		symbol.symbolDynamicData.VWAP = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"VWAP"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"AvgVol"]) {
		symbol.symbolDynamicData.averageVolume = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"AvgVol"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"AvgVal"]) { 
		symbol.symbolDynamicData.averageValue = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"AvgVal"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"Status"]) { 
		symbol.symbolDynamicData.tradingStatus = [updateDictionary objectForKey:@"Status"];
	}
	if ([updateDictionary objectForKey:@"B Lot"]) {
		NSString *bidLot = [updateDictionary objectForKey:@"B Lot"];
		NSUInteger multiplier = 1;
		if ([bidLot rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000;
		} else if ([bidLot rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000;
		}
		symbol.symbolDynamicData.buyLot = [NSNumber numberWithInteger:[bidLot integerValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"BLValue"]) { 
		symbol.symbolDynamicData.buyLotValue = [NSNumber numberWithDouble:[[updateDictionary objectForKey:@"BLValue"] doubleValue]];
	}
	if ([updateDictionary objectForKey:@"Shares"]) {
		NSString *shares = [updateDictionary objectForKey:@"Shares"];
		NSUInteger multiplier = 1;
		if ([shares rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000;
		} else if ([shares rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000;
		}
		symbol.symbolDynamicData.outstandingShares = [NSNumber numberWithInteger:[shares integerValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"M Cap"]) {
		NSString *marketCapitalizationString = [updateDictionary objectForKey:@"M Cap"];
		NSLog(@"MARKET CAP IS %@", marketCapitalizationString);
		float multiplier = 1.0;
		if ([marketCapitalizationString rangeOfString:@"k"].location != NSNotFound) {
			multiplier = 1000.0;
		} else if ([marketCapitalizationString rangeOfString:@"m"].location != NSNotFound) {
			multiplier = 1000000.0;
		}
		symbol.symbolDynamicData.marketCapitalization = [NSNumber numberWithDouble:[marketCapitalizationString doubleValue] * multiplier];
	}
	if ([updateDictionary objectForKey:@"Exchange"]) {
		//
	}
	if ([updateDictionary objectForKey:@"Country"]) {
		symbol.country = [updateDictionary objectForKey:@"Country"];
	}
	if ([updateDictionary objectForKey:@"Description"]) {
		//
	}
	if ([updateDictionary objectForKey:@"Symbol"]) {
		//
	}
	if ([updateDictionary objectForKey:@"ISIN"]) { 
		//
	}
	if ([updateDictionary objectForKey:@"Currency"]) { 
		symbol.currency = [updateDictionary objectForKey:@"Currency"];
	}
}

- (void)orderBookControllerDidFinish:(OrderBookController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)tradesControllerDidFinish:(TradesController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)chartControllerDidFinish:(ChartController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)symbolNewsControllerDidFinish:(SymbolNewsController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber {
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed.feedNumber=%@) AND (tickerSymbol=%@)", feedNumber, tickerSymbol];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

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
	[self.symbol removeObserver:self forKeyPath:@"currency"];
	[self.symbol removeObserver:self forKeyPath:@"symbolDynamicData.lastTrade"];
	
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
	[lastTradeChange release];
	[lastTradePercentChange release];
	[buyLot release];
	[buyLotValue release];
	[trades release];
	[tradingStatus release];
	[averageValue release];
	[averageVolume release];
	[onVolume release];
	
	
	[dateFormatter release];
	[timeFormatter release];
	[doubleFormatter release];
	[integerFormatter release];
	[percentFormatter release];
	
	[mainFont release];
	[headerFont release];
	
	[toolBar release];
	
	[scrollView release];
	[self.symbol release];
	[managedObjectContext release];
	[super dealloc];
}

@end
