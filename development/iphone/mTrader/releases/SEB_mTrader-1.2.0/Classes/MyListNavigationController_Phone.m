//
//  MyListNavigationController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "MyListNavigationController_Phone.h"

#import "mTraderAppDelegate_Phone.h"

#import "MyListHeaderViewController_Phone.h"
#import "SymbolDetailController.h"


@implementation MyListNavigationController_Phone

- (id)initWithContentViewController:(UIViewController *)rootViewController {
	self = [super initWithRootViewController:rootViewController];
	if (self != nil) {
		UIImage* anImage = [UIImage imageNamed:@"MyListTab.png"];	
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"MyListTab", "My List tab label")  image:anImage tag:CHAINS];
		self.tabBarItem = theItem;
		[theItem release];
	}
	return self;
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	NSString *applicationName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
	if ([applicationName isEqualToString:BRANDING_SEB]) {
		self.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	}

}

- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated {

	if ([viewController isMemberOfClass:MyListHeaderViewController_Phone.class]) {
		MyListHeaderViewController_Phone *myListHeaderViewController = (MyListHeaderViewController_Phone *)viewController;
		[myListHeaderViewController changeQFieldsStreaming];
	} else if ([viewController isMemberOfClass:SymbolDetailController.class]) {
		SymbolDetailController *symbolDetailController = (SymbolDetailController *)viewController;
		[symbolDetailController changeQFieldsStreaming];
	}
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
#if DEBUG
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in MyListNavigationController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

- (void)dealloc {
    [super dealloc];
}


@end
