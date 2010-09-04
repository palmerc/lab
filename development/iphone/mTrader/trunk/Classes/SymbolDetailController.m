//
//  SymbolDetailController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "SymbolDetailController.h"

#import <QuartzCore/QuartzCore.h>

#import "mTraderCommunicator.h"
#import "DataController.h"

#import "LastChangeView.h"
#import "TradesLiveInfoView.h"
#import "OrderBookView.h"
#import "SymbolNewsController.h"
#import "OtherInfoView.h"
#import "ScrollViewPageControl.h"

#import "RoundedRectangleFrame.h"
#import "RoundedRectangleFrame.h"
#import "OrderBookController.h"
#import "OrderBookView.h"
#import "PastTradesController.h"
#import "ChartController.h";
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "QFields.h"

@implementation SymbolDetailController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize symbol = _symbol;

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
    if (self = [super init]) {
		self.symbol = symbol;
	}
    return self;
}

- (void)loadView {
	CGRect applicationFrame = [[UIScreen mainScreen] applicationFrame];
	UIView *aView = [[UIView alloc] initWithFrame:applicationFrame];
	aView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	aView.autoresizesSubviews = YES;
	
	aView.backgroundColor = [UIColor groupTableViewBackgroundColor];
	
	CGFloat halfWidth = applicationFrame.size.width / 2.0f;
	/*** Last ***/
	CGRect lastRoundedFrame = CGRectMake(0.0, 0.0, halfWidth, 220.0f);
	CGRect lastInnerFrame = CGRectMake(10.0f, 10.0f, halfWidth - 20.f, 200.0f);
	
	_lastChangeView = [[LastChangeView alloc] initWithFrame:lastInnerFrame];
	_lastChangeView.symbol = self.symbol;
	
	[_lastChangeView.chartButton addTarget:self action:@selector(chartTouchedUpInside:) forControlEvents:UIControlEventTouchUpInside];
	
	RoundedRectangleFrame *lastBox = [[RoundedRectangleFrame alloc] initWithFrame:lastRoundedFrame];
	lastBox.strokeWidth = 0.75f;
	lastBox.cornerRadius = 10.0f;
	lastBox.padding = 6.0f;
	lastBox.backgroundColor = [UIColor clearColor];
	[lastBox addSubview:_lastChangeView];

	[aView addSubview:lastBox];
	[lastBox release];
		
	/*** Trades Information ****/
	CGRect tradesRoundedFrame = CGRectMake(applicationFrame.size.width / 2.0f, 0.0, halfWidth, 220.0f);
	CGRect tradesInnerFrame = CGRectMake(10.0f, 10.0f, halfWidth - 20.f, 200.0f);
	
	_tradesLiveBox = [[TradesLiveInfoView alloc] initWithFrame:tradesInnerFrame];
	_tradesLiveBox.symbol = self.symbol;
	
	RoundedRectangleFrame *tradesBox = [[RoundedRectangleFrame alloc] initWithFrame:tradesRoundedFrame];
	tradesBox.strokeWidth = 0.75f;
	tradesBox.cornerRadius = 10.0f;
	tradesBox.padding = 6.0f;
	tradesBox.backgroundColor = [UIColor clearColor];
	[tradesBox addSubview:_tradesLiveBox];

	[aView addSubview:tradesBox];
	[tradesBox release];	
	
	CGRect detailRoundedFrame = CGRectMake(0.0, 0.0, applicationFrame.size.width, applicationFrame.size.height - tradesRoundedFrame.size.height);
	CGRect detailInnerFrame = CGRectMake(10.0f, 10.0f, detailRoundedFrame.size.width - 20.0f, detailRoundedFrame.size.height - 20.0f);
	CGRect detailInnerBounds = detailInnerFrame;
	detailInnerBounds.origin.x = 0;
	detailInnerBounds.origin.y = 0;
	
	/*** Orderbook ***/
	RoundedRectangleFrame *orderBookBox = [[RoundedRectangleFrame alloc] initWithFrame:detailRoundedFrame];
	orderBookBox.strokeWidth = 0.75f;
	orderBookBox.cornerRadius = 10.0f;
	orderBookBox.padding = 6.0f;
	orderBookBox.backgroundColor = [UIColor clearColor];
	
	_orderBookController = [[OrderBookController alloc] initWithSymbol:_symbol];
	_orderBookController.managedObjectContext = _managedObjectContext;
	
	UIView *orderBookView = _orderBookController.view;
	orderBookView.frame = detailInnerBounds;
	
	UIButton *orderBookButton = [[UIButton alloc] initWithFrame:detailInnerFrame];
	[orderBookButton addTarget:self action:@selector(orderBookTouchedUpInside:) forControlEvents:UIControlEventTouchUpInside];
	[orderBookButton addSubview:orderBookView];
	[orderBookBox addSubview:orderBookButton];
	[orderBookButton release];
	
	/*** Historic Trades ***/
	RoundedRectangleFrame *pastTradesBox = [[RoundedRectangleFrame alloc] initWithFrame:detailRoundedFrame];
	pastTradesBox.strokeWidth = 0.75f;
	pastTradesBox.cornerRadius = 10.0f;
	pastTradesBox.padding = 6.0f;
	pastTradesBox.backgroundColor = [UIColor clearColor];
	
	_pastTradesController = [[PastTradesController alloc] initWithSymbol:_symbol];
	_pastTradesController.managedObjectContext = _managedObjectContext;
	
	UIView *pastTradesView = _pastTradesController.view;
	pastTradesView.frame = detailInnerBounds;
	
	UIButton *pastTradesButton = [[UIButton alloc] initWithFrame:detailInnerFrame];
	[pastTradesButton addTarget:self action:@selector(pastTradesTouchedUpInside:) forControlEvents:UIControlEventTouchUpInside];
	[pastTradesButton addSubview:pastTradesView];
	[pastTradesBox addSubview:pastTradesButton];
	[pastTradesButton release];
	
	/*** Symbol News Box ***/
	RoundedRectangleFrame *newsBox = [[RoundedRectangleFrame alloc] initWithFrame:detailRoundedFrame];
	newsBox.strokeWidth = 0.75f;
	newsBox.cornerRadius = 10.0f;
	newsBox.padding = 6.0f;
	newsBox.backgroundColor = [UIColor clearColor];
	
	_symbolNewsController = [[SymbolNewsController alloc] initWithSymbol:self.symbol];
	_symbolNewsController.managedObjectContext = _managedObjectContext;
	
	UIView *symbolNewsView = _symbolNewsController.view;
	symbolNewsView.frame = detailInnerBounds;
	
	UIButton *symbolNewsButton = [[UIButton alloc] initWithFrame:detailInnerFrame];
	[symbolNewsButton addTarget:self action:@selector(symbolNewsTouchedUpInside:) forControlEvents:UIControlEventTouchUpInside];
	[symbolNewsButton addSubview:symbolNewsView];
	[newsBox addSubview:symbolNewsButton];
	[symbolNewsButton release];
	
	/*** SEB Analysis ***/
	RoundedRectangleFrame *analysisBox = nil;
	NSString *providerURL = _symbol.symbolDynamicData.providerURL;
	if (providerURL != nil) {		
		analysisBox = [[RoundedRectangleFrame alloc] initWithFrame:detailRoundedFrame];
		analysisBox.strokeWidth = 0.75f;
		analysisBox.cornerRadius = 10.0f;
		analysisBox.padding = 6.0f;
		analysisBox.backgroundColor = [UIColor clearColor];
		
		UIView *analysisView = [[UIView alloc] initWithFrame:detailInnerFrame];
		[analysisBox addSubview:analysisView];

		UIImage *logo = [UIImage imageNamed:@"sebLogo"];
		CGFloat logoWidth = logo.size.width;
		CGFloat logoHeight = logo.size.height;
		CGFloat x = floorf((detailInnerFrame.size.width - logoWidth) / 2.0f);
		UIImageView *logoView = [[UIImageView alloc] initWithImage:logo];
		CGRect logoViewFrame = logoView.frame;
		logoViewFrame.origin.x = x;
		logoView.frame = logoViewFrame;
		
		UIButton *analysisButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		[analysisButton addTarget:self action:@selector(analysisButtonTouchedUpInside:) forControlEvents:UIControlEventTouchUpInside];
		[analysisButton setTitle:@"Pre-trade Report" forState:UIControlStateNormal];

		[analysisView addSubview:analysisButton];
		
		analysisButton.frame = CGRectMake(10.0f, logoHeight + 5.0f, detailInnerFrame.size.width - 20.0f, 50.0f);
		
		[analysisView addSubview:logoView];
	}
	
	/*** Other Box ***/
	RoundedRectangleFrame *otherInfoBox = [[RoundedRectangleFrame alloc] initWithFrame:detailRoundedFrame];
	otherInfoBox.strokeWidth = 0.75f;
	otherInfoBox.cornerRadius = 10.0f;
	otherInfoBox.padding = 6.0f;
	otherInfoBox.backgroundColor = [UIColor clearColor];
	
	OtherInfoView *otherInfoView = [[OtherInfoView alloc] initWithFrame:detailInnerFrame];
	otherInfoView.symbol = self.symbol;
	[otherInfoBox addSubview:otherInfoView];
	[otherInfoView release];
		
	CGRect detailBoxFrame = CGRectMake(0.0, lastRoundedFrame.size.height, applicationFrame.size.width, applicationFrame.size.height - lastRoundedFrame.size.height);
	_detailBox = [[ScrollViewPageControl alloc] init];
	_detailBox.view.frame = detailBoxFrame;

	if (providerURL != nil) {
		_detailBox.views = [NSArray arrayWithObjects:orderBookBox, pastTradesBox, newsBox, analysisBox, otherInfoBox, nil];
	} else {
		_detailBox.views = [NSArray arrayWithObjects:orderBookBox, pastTradesBox, newsBox, otherInfoBox, nil];
	}
	
	[orderBookBox release];
	[pastTradesBox release];
	[newsBox release];
	[analysisBox release];
	[otherInfoBox release];
	
	[aView addSubview:_detailBox.view];

	self.view = aView;
	[aView release];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];

	[self changeQFieldsStreaming];
}

- (void)changeQFieldsStreaming {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
		
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", self.symbol.feed.feedNumber, self.symbol.tickerSymbol];
	
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
	[communicator symbolNewsForFeedTicker:feedTicker];
}

#pragma mark -
#pragma mark SymbolNews Modal View delegate methods

- (void)symbolNewsTouchedUpInside:(id)sender {
	SymbolNewsController *symbolNewsController = [[SymbolNewsController alloc] initWithSymbol:_symbol];
	symbolNewsController.managedObjectContext = _managedObjectContext;
	
	symbolNewsController.modal = YES;
	symbolNewsController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	symbolNewsController.title = [NSString stringWithFormat:@"%@ (%@)", _symbol.tickerSymbol, _symbol.feed.mCode];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(symbolNewsModalDidFinish:)];
	symbolNewsController.navigationItem.leftBarButtonItem = doneButton;
	
	[doneButton release];
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:symbolNewsController];
	NSString *applicationName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
	if ([applicationName isEqualToString:BRANDING_SEB]) {
		navController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	}
	[symbolNewsController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)symbolNewsModalDidFinish:(id)sender {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)analysisButtonTouchedUpInside:(id)sender {
	NSURL *url = [NSURL URLWithString:self.symbol.symbolDynamicData.providerURL];
	[[UIApplication sharedApplication] openURL:url];
}

- (void)chartTouchedUpInside:(id)sender {
	ChartController *chartController = [[ChartController alloc] init];
	chartController.symbol = _symbol;
	
	chartController.modal = YES;
	chartController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	chartController.title = [NSString stringWithFormat:@"%@ (%@)", _symbol.tickerSymbol, _symbol.feed.mCode];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(chartModalDidFinish:)];
	chartController.navigationItem.leftBarButtonItem = doneButton;
	
	[doneButton release];
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:chartController];
	NSString *applicationName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
	if ([applicationName isEqualToString:BRANDING_SEB]) {
		navController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	}
	[chartController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)chartModalDidFinish:(id)sender {
	_lastChangeView.chartController.period = 0;
	[self dismissModalViewControllerAnimated:YES];
}

#pragma mark -
#pragma mark PastTrades Modal View delegate methods

- (void)pastTradesTouchedUpInside:(id)sender {
	PastTradesController *pastTradesController = [[PastTradesController alloc] initWithSymbol:_symbol];
	pastTradesController.managedObjectContext = _managedObjectContext;

	pastTradesController.modal = YES;
	pastTradesController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	pastTradesController.title = [NSString stringWithFormat:@"%@ (%@)", _symbol.tickerSymbol, _symbol.feed.mCode];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(pastTradesModalDidFinish:)];
	pastTradesController.navigationItem.leftBarButtonItem = doneButton;
	
	[doneButton release];
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:pastTradesController];
	NSString *applicationName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
	if ([applicationName isEqualToString:BRANDING_SEB]) {
		navController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	}
	[pastTradesController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)pastTradesModalDidFinish:(id)sender {
	[self dismissModalViewControllerAnimated:YES];
}


#pragma mark -
#pragma mark OrderBook Modal View delegate methods

- (void)orderBookTouchedUpInside:(id)sender {
	OrderBookController *orderBookController = [[OrderBookController alloc] initWithSymbol:_symbol];
	orderBookController.managedObjectContext = _managedObjectContext;
	
	orderBookController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	orderBookController.title = [NSString stringWithFormat:@"%@ (%@)", _symbol.tickerSymbol, _symbol.feed.mCode];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(orderBookModalDidFinish:)];
	orderBookController.navigationItem.leftBarButtonItem = doneButton;
	
	[doneButton release];
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:orderBookController];	
	NSString *applicationName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
	if ([applicationName isEqualToString:BRANDING_SEB]) {
		navController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	}
	[orderBookController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}


- (void)orderBookModalDidFinish:(id)sender {	
	[self dismissModalViewControllerAnimated:YES];
}

#pragma mark -
#pragma mark Modal view finished methods

- (void)analysisModalViewControllerDidFinish:(UIViewController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}


#pragma mark -
#pragma mark Debugging methods
#if DEBUG
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"SymbolDetailController: respondsToSelector %@", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_lastChangeView release];
	[_tradesLiveBox release];
	[_detailBox release];
	
	[_symbol release];
	[_managedObjectContext release];
	[super dealloc];
}

@end
