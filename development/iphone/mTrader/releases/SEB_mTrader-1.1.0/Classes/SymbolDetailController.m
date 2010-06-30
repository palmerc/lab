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
@synthesize modalTransitionComplete = _modalTransitionComplete;

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
    if (self = [super init]) {
		_symbol = [symbol retain];
		_modalTransitionComplete = YES;
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
	newsBox.viewController = self;
	
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
	if (self.modalTransitionComplete == NO) {
		return;
	}
	self.modalTransitionComplete = NO;
	OrderBookModalController *orderBookController = [[OrderBookModalController alloc] initWithManagedObjectContext:self.managedObjectContext];
	orderBookController.symbol = self.symbol;
	orderBookController.delegate = self;
	orderBookController.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:orderBookController];
	navController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	[orderBookController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)trades:(id)sender {
	if (self.modalTransitionComplete == NO) {
		return;
	}
	self.modalTransitionComplete = NO;
	CGRect frame = CGRectMake(0.0f, 0.0f, 320.0f, 416.0f);
	TradesController *tradesController = [[TradesController alloc] initWithFrame:frame];
	tradesController.managedObjectContext = self.managedObjectContext;
	tradesController.symbol = self.symbol;
	tradesController.delegate = self;
	tradesController.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:tradesController];
	navController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	[tradesController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)chart:(id)sender {
	if (self.modalTransitionComplete == NO) {
		return;
	}
	self.modalTransitionComplete = NO;
	ChartController *chartController = [[ChartController alloc] initWithSymbol:self.symbol];
	chartController.delegate = self;
	chartController.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:chartController];
	navController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	[chartController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}


- (void)news:(id)sender {
	if (self.modalTransitionComplete == NO) {
		return;
	}
	self.modalTransitionComplete = NO;
	SymbolNewsModalController *symbolNewsController = [[SymbolNewsModalController alloc] initWithManagedObjectContext:self.managedObjectContext];
	symbolNewsController.symbol = self.symbol;
	symbolNewsController.delegate = self;
	symbolNewsController.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:symbolNewsController];
	navController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	[symbolNewsController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

- (void)orderBookModalControllerDidFinish:(OrderBookModalController *)controller {
	[self dismissModalViewControllerAnimated:YES];
	self.modalTransitionComplete = YES;
}

- (void)tradesControllerDidFinish:(TradesController *)controller {
	[self dismissModalViewControllerAnimated:YES];
	self.modalTransitionComplete = YES;
}

- (void)chartControllerDidFinish:(ChartController *)controller {
	[self dismissModalViewControllerAnimated:YES];

	[lastBox setNeedsDisplay];
	self.modalTransitionComplete = YES;
}

- (void)symbolNewsModalControllerDidFinish:(SymbolNewsModalController *)controller {
	[self dismissModalViewControllerAnimated:YES];
	self.modalTransitionComplete = YES;
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
