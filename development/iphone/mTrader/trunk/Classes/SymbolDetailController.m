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
#import "PastTradesController.h"
#import "ChartController.h";
#import "SymbolNewsModalController_Phone.h"
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
	
	aView.backgroundColor = [UIColor groupTableViewBackgroundColor];
	
	CGFloat halfWidth = applicationFrame.size.width / 2.0f;
	/*** Last ***/
	CGRect lastRoundedFrame = CGRectMake(0.0, 0.0, halfWidth, 220.0f);
	CGRect lastInnerFrame = CGRectMake(10.0f, 10.0f, halfWidth - 20.f, 200.0f);
	
	_lastChangeView = [[LastChangeView alloc] initWithFrame:lastInnerFrame];
	_lastChangeView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	_lastChangeView.autoresizesSubviews = YES;
	_lastChangeView.symbol = self.symbol;	
	
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
	_tradesLiveBox.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	_tradesLiveBox.autoresizesSubviews = YES;
	
	RoundedRectangleFrame *tradesBox = [[RoundedRectangleFrame alloc] initWithFrame:tradesRoundedFrame];
	tradesBox.strokeWidth = 0.75f;
	tradesBox.cornerRadius = 10.0f;
	tradesBox.padding = 6.0f;
	tradesBox.backgroundColor = [UIColor clearColor];
	[tradesBox addSubview:_tradesLiveBox];

	[aView addSubview:tradesBox];
	[tradesBox release];	
	
	CGRect detailRoundedFrame = CGRectMake(0.0, 0.0, applicationFrame.size.width, applicationFrame.size.height - tradesRoundedFrame.size.height - 90.0f);
	CGRect detailInnerFrame = CGRectMake(10.0f, 10.0f, detailRoundedFrame.size.width - 20.0f, detailRoundedFrame.size.height - 20.0f);
		
	/*** Orderbook ***/
	RoundedRectangleFrame *orderBookBox = [[RoundedRectangleFrame alloc] initWithFrame:detailRoundedFrame];
	orderBookBox.strokeWidth = 0.75f;
	orderBookBox.cornerRadius = 10.0f;
	orderBookBox.padding = 6.0f;
	orderBookBox.backgroundColor = [UIColor clearColor];
	
	_orderBookController = [[OrderBookController alloc] initWithSymbol:self.symbol];
	_orderBookController.managedObjectContext = _managedObjectContext;
	UIView *orderBookView = _orderBookController.view;
	orderBookView.frame = detailInnerFrame;
	orderBookView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	orderBookView.autoresizesSubviews = YES;
	[orderBookBox addSubview:orderBookView];
	
	/*** Historic Trades ***/
	RoundedRectangleFrame *pastTradesBox = [[RoundedRectangleFrame alloc] initWithFrame:detailRoundedFrame];
	pastTradesBox.strokeWidth = 0.75f;
	pastTradesBox.cornerRadius = 10.0f;
	pastTradesBox.padding = 6.0f;
	pastTradesBox.backgroundColor = [UIColor clearColor];
	
	_pastTradesController = [[PastTradesController alloc] initWithSymbol:self.symbol];
	_pastTradesController.managedObjectContext = _managedObjectContext;
	UIView *pastTradesView = _pastTradesController.view;
	pastTradesView.frame = detailInnerFrame;
	pastTradesView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	pastTradesView.autoresizesSubviews = YES;
	[pastTradesBox addSubview:pastTradesView];
	
	/*** Symbol News Box ***/
	RoundedRectangleFrame *newsBox = [[RoundedRectangleFrame alloc] initWithFrame:detailRoundedFrame];
	newsBox.strokeWidth = 0.75f;
	newsBox.cornerRadius = 10.0f;
	newsBox.padding = 6.0f;
	newsBox.backgroundColor = [UIColor clearColor];
	
	_symbolNewsController = [[SymbolNewsController alloc] initWithSymbol:self.symbol];
	_symbolNewsController.managedObjectContext = _managedObjectContext;
	UIView *symbolNewsView = _symbolNewsController.view;
	symbolNewsView.frame = detailInnerFrame;
	symbolNewsView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	symbolNewsView.autoresizesSubviews = YES;
	[newsBox addSubview:symbolNewsView];
	
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
		
	CGRect detailFrame = CGRectMake(0.0, 210.f, applicationFrame.size.width, applicationFrame.size.height - 120.0f);
	_detailBox = [[ScrollViewPageControl alloc] initWithFrame:detailFrame];
	_detailBox.views = [NSArray arrayWithObjects:orderBookBox, pastTradesBox, newsBox, otherInfoBox, nil];
	[orderBookBox release];
	[pastTradesBox release];
	[newsBox release];
	[otherInfoBox release];
	
	[aView addSubview:_detailBox.view];

	self.view = aView;
	[aView release];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
	if (self.symbol.symbolDynamicData.providerURL != nil) {
		NSString *analysisString = NSLocalizedString(@"analysis", @"Analysis");
		UIBarButtonItem *analysis = [[UIBarButtonItem alloc] initWithTitle:analysisString style:UIBarButtonItemStylePlain target:self action:@selector(analysis:)];
		self.navigationItem.rightBarButtonItem = analysis;
		[analysis release];
	}
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];

	[self changeQFieldsStreaming];
}

- (void)changeQFieldsStreaming {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	[[DataController sharedManager] deleteAllBidsAsks];
	
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
#pragma mark Action methods

- (void)orderBook:(id)sender {
	OrderBookModalController *orderBookController = [[OrderBookModalController alloc] initWithManagedObjectContext:self.managedObjectContext];
	orderBookController.symbol = self.symbol;
	orderBookController.delegate = self;
	orderBookController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:orderBookController];
	[orderBookController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)trades:(id)sender {
	TradesModalController *tradesController = [[TradesModalController alloc] initWithManagedObjectContext:self.managedObjectContext];
	tradesController.symbol = self.symbol;
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
		
	[self presentModalViewController:chartController animated:YES];
	[chartController release];
}


- (void)news:(id)sender {
	SymbolNewsModalController *symbolNewsController = [[SymbolNewsModalController alloc] initWithManagedObjectContext:self.managedObjectContext];
	symbolNewsController.symbol = self.symbol;
	symbolNewsController.delegate = self;
	symbolNewsController.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:symbolNewsController];
	[symbolNewsController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)analysis:(id)sender {
	NSURL *url = [NSURL URLWithString:self.symbol.symbolDynamicData.providerURL];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	UIWebView *analysisView = [[UIWebView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	UIViewController *viewController = [[UIViewController alloc] init];
	
	UIBarButtonItem *done = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(analysisModalViewControllerDidFinish:)];
	viewController.navigationItem.leftBarButtonItem = done;
	[viewController.view addSubview:analysisView];
	[analysisView loadRequest:request];
	[analysisView release];
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:viewController];
	[viewController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

#pragma mark -
#pragma mark Modal view finished methods

- (void)orderBookModalControllerDidFinish:(OrderBookModalController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)tradesControllerDidFinish:(TradesModalController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)chartControllerDidFinish:(ChartController *)controller {
	[self dismissModalViewControllerAnimated:YES];

	[_lastChangeView setNeedsDisplay];
}

- (void)symbolNewsModalControllerDidFinish:(SymbolNewsModalController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

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
