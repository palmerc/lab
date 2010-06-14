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
#import "DataController.h"
#import "StringHelpers.h"

#import "LastChangeView.h"
#import "TradesLiveInfoView.h"
#import "TradesInfoView.h"
#import "OrderBookView.h"
#import "SymbolNewsView.h"
#import "OtherInfoView.h"
#import "ScrollViewPageControl.h"

#import "RoundedRectangle.h"
#import "OrderBookController.h"
#import "TradesController.h"
#import "ChartController.h";
#import "SymbolNewsModalController.h"
#import "Feed.h"
#import "Symbol.h"
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

- (void)viewDidLoad {
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	CGRect windowFrame = self.view.bounds;
	
	CGRect lastFrame = CGRectMake(0.0, 0.0, windowFrame.size.width / 2.0f, 220.0f);
	_lastBox = [[LastChangeView alloc] initWithFrame:lastFrame];
	_lastBox.symbol = self.symbol;

	CGRect tradesFrame = CGRectMake(windowFrame.size.width / 2.0f, 0.0, windowFrame.size.width / 2.0f, 220.0f);
	_tradesLiveBox = [[TradesLiveInfoView alloc] initWithFrame:tradesFrame];
	_tradesLiveBox.symbol = self.symbol;
	
	CGRect roundedFrame = CGRectMake(0.0, 0.0, windowFrame.size.width, windowFrame.size.height - tradesFrame.size.height - 90.0f);
	_orderBox = [[OrderBookView alloc] initWithFrame:roundedFrame andManagedObjectContext:self.managedObjectContext];
	_orderBox.symbol = self.symbol;
	
	_tradesBox = [[TradesInfoView alloc] initWithFrame:roundedFrame andManagedObjectContext:self.managedObjectContext];
	_tradesBox.symbol = self.symbol;
	
	_newsBox = [[SymbolNewsView alloc] initWithFrame:roundedFrame andManagedObjectContext:self.managedObjectContext];
	_newsBox.symbol = self.symbol;
	
	_otherBox = [[OtherInfoView alloc] initWithFrame:roundedFrame];
	_otherBox.symbol = self.symbol;
	
	CGRect detailFrame = CGRectMake(0.0, 220.f, windowFrame.size.width, windowFrame.size.height - 120.0f);
	_detailBox = [[ScrollViewPageControl alloc] initWithFrame:detailFrame];
	_detailBox.views = [NSArray arrayWithObjects:_orderBox, _tradesBox, _newsBox, _otherBox, nil];
	
	[self.view addSubview:_lastBox];
	[self.view addSubview:_tradesLiveBox];
	[self.view addSubview:_detailBox.view];
}

- (void)viewWillAppear:(BOOL)animated {
	[self changeQFieldsStreaming];
}

- (void)changeQFieldsStreaming {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	[[DataController sharedManager] deleteAllNews];
	[[DataController sharedManager] deleteAllBidsAsks];
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	
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
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:chartController];
	[chartController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
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

	[_lastBox setNeedsDisplay];
}

- (void)symbolNewsModalControllerDidFinish:(SymbolNewsModalController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}


#pragma mark -
#pragma mark Debugging methods
//// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	NSLog(@"Queried about %@ in SymbolDetailController", NSStringFromSelector(sel));
//	return [super respondsToSelector:sel];
//}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_lastBox release];
	[_tradesLiveBox release];
	[_tradesBox release];
	[_orderBox release];
	[_newsBox release];
	[_detailBox release];
	
	[_symbol release];
	[_managedObjectContext release];
	[super dealloc];
}

@end
