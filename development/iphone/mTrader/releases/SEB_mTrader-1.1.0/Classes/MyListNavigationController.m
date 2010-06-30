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
	
	self.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	UIView *contentView = self.view;
	CGRect frame = self.view.frame;
	UIView *aView = [[UIView alloc] initWithFrame:frame];
	[aView addSubview:contentView];
	[contentView addSubview:self.myListViewController.view];

	self.view = aView;
	[aView release];
	[self pushViewController:self.myListViewController animated:NO];
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
	self.myListViewController = nil;
}

- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated {

	if ([viewController isMemberOfClass:MyListViewController.class]) {
		MyListViewController *myListViewController = (MyListViewController *)viewController;
		[myListViewController changeQFieldsStreaming];
	} else if ([viewController isMemberOfClass:SymbolDetailController.class]) {
		SymbolDetailController *symbolDetailController = (SymbolDetailController *)viewController;
		[symbolDetailController changeQFieldsStreaming];
	}
}

//#pragma mark -
//#pragma mark Debugging methods
//// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	NSLog(@"Queried about %@ in MyListNavigationController", NSStringFromSelector(sel));
//	return [super respondsToSelector:sel];
//}

- (void)dealloc {
	[_myListViewController release];

    [super dealloc];
}


@end
