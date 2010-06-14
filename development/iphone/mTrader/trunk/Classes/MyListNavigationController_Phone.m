//
//  MyListNavigationController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


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

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

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
//// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	NSLog(@"Queried about %@ in MyListNavigationController", NSStringFromSelector(sel));
//	return [super respondsToSelector:sel];
//}

- (void)dealloc {
    [super dealloc];
}


@end
