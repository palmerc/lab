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
#import "SymbolDataController.h"
#import "StringHelpers.h"

#import "LastChangeView.h"
#import "TradesInfoView.h"
#import "OrderBookView.h"
#import "SymbolNewsView.h"

#import "RoundedRectangle.h"
#import "OrderBookController.h"
#import "TradesController.h"
#import "ChartController.h";
#import "SymbolNewsModalController.h"
#import "Feed.h"
#import "Symbol.h"
#import "QFields.h"

@implementation SymbolDetailController
@synthesize managedObjectContext;
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
	[super viewDidLoad];
	
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
	UIScrollView* containerView = [[UIScrollView alloc] initWithFrame:self.view.frame];
	containerView.backgroundColor = [UIColor groupTableViewBackgroundColor];
	containerView.scrollEnabled = YES;
	containerView.bounces = NO;
	containerView.contentSize = CGSizeMake(self.view.frame.size.width, 520.0f);
	self.view = containerView;
	
	CGRect lastFrame = CGRectMake(0.0, 0.0, 160.0, 220.0);
	lastBox = [[LastChangeView alloc] initWithFrame:lastFrame];
	lastBox.symbol = self.symbol;

	CGRect tradesFrame = CGRectMake(160.0, 0.0, 160.0, 220.0);
	tradesBox = [[TradesInfoView alloc] initWithFrame:tradesFrame];
	tradesBox.symbol = self.symbol;
	
	CGRect orderFrame = CGRectMake(0.0, 220.0, 320.0, 150.0);
	orderBox = [[OrderBookView alloc] initWithFrame:orderFrame andManagedObjectContext:self.managedObjectContext];
	orderBox.symbol = self.symbol;
	
	CGRect newsFrame = CGRectMake(0.0, 370.0, 320.0, 150.0);
	newsBox = [[SymbolNewsView alloc] initWithFrame:newsFrame andManagedObjectContext:self.managedObjectContext];
	newsBox.symbol = self.symbol;
	
	[self.view addSubview:lastBox];
	[self.view addSubview:tradesBox];
	[self.view addSubview:orderBox];
	[self.view addSubview:newsBox];
}

- (void)viewDidAppear:(BOOL)animated {
	[super viewDidAppear:animated];
	
	[self changeQFieldsStreaming];
}

- (void)changeQFieldsStreaming {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	[[SymbolDataController sharedManager] deleteAllNews];
	[[SymbolDataController sharedManager] deleteAllBidsAsks];
	
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
	TradesController *tradesController = [[TradesController alloc] initWithManagedObjectContext:self.managedObjectContext];
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

- (void)orderBookModalControllerDidFinish:(OrderBookModalController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)tradesControllerDidFinish:(TradesController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)chartControllerDidFinish:(ChartController *)controller {
	[self dismissModalViewControllerAnimated:YES];

	[lastBox setNeedsDisplay];
}

- (void)symbolNewsModalControllerDidFinish:(SymbolNewsModalController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}


//#pragma mark -
//#pragma mark Debugging methods
//// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	NSLog(@"Queried about %@ in SymbolDetailController", NSStringFromSelector(sel));
//	return [super respondsToSelector:sel];
//}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[lastBox release];
	[tradesBox release];
	[orderBox release];
	[newsBox release];
	
	[_symbol release];
	[managedObjectContext release];
	[super dealloc];
}

@end
