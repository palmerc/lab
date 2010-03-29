//
//  MyListNavigationController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "MyListNavigationController.h"

#import "mTraderAppDelegate.h"

#import "SymbolDataController.h"

#import "MyListViewController.h"
#import "SymbolDetailController.h"

#import "QFields.h"
#import "Feed.h"
#import "Symbol.h"

@implementation MyListNavigationController
@synthesize myListViewController = _myListViewController;

- (id)initWithContentViewController:(UIViewController *)rootViewController {
	self = [super init];
	if (self != nil) {
		_myListViewController = [rootViewController retain];
		self.myListViewController.navigationController = self;
		self.delegate = self;
		UIImage* anImage = [UIImage imageNamed:@"MyListTab.png"];	
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"MyStocksTab", "My Stocks tab label")  image:anImage tag:CHAINS];
		self.tabBarItem = theItem;
		[theItem release];
	}
	return self;
}

- (void)loadView {
	[super loadView];
	
	UIView *contentView = self.view;
	CGRect frame = self.view.frame;
	UIView *aView = [[UIView alloc] initWithFrame:frame];
	[aView addSubview:contentView];
	[contentView addSubview:self.myListViewController.view];
	
	self.view = aView;
	[aView release];
	[self pushViewController:self.myListViewController animated:NO];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
	self.myListViewController = nil;
}

- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];

	if ([viewController isMemberOfClass:MyListViewController.class]) {
		QFields *qFields = [[QFields alloc] init];
		qFields.timeStamp = YES;
		qFields.lastTrade = YES;
		qFields.bidPrice = YES;
		qFields.askPrice = YES;
		qFields.change = YES;
		qFields.changePercent = YES;
		communicator.qFields = qFields;
		[qFields release];
		
		[communicator setStreamingForFeedTicker:nil];
	} else if ([viewController isMemberOfClass:SymbolDetailController.class]) {
		Symbol *symbol = ((SymbolDetailController *)viewController).symbol;
		
		[[SymbolDataController sharedManager] deleteAllBidsAsks];
		
		NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [symbol.feed.feedNumber stringValue], symbol.tickerSymbol];

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
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in MyListNavigationController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}

- (void)dealloc {
	[_myListViewController release];

    [super dealloc];
}


@end
