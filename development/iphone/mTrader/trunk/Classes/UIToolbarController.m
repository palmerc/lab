//
//  UIToolbarController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 10.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "UIToolbarController.h"


@implementation UIToolbarController
@synthesize contentViewController = _contentViewController;
@synthesize toolBar = _toolBar;

- (id)initWithContentViewController:(UIViewController *)contentViewController {
	self = [super init];
	
	if (self) {
		_contentViewController = [contentViewController retain];
		if ([_contentViewController isKindOfClass:[UINavigationController class]]) {
			((UINavigationController*)_contentViewController).delegate = self;
		}
	}
	
	return self;
}

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	UIView *contentView = _contentViewController.view;
	
	CGRect frame = contentView.frame;
	UIView *view = [[UIView alloc] initWithFrame:frame];
	
	frame = CGRectMake(0.0f, 0.0f, frame.size.width, frame.size.height - 44.0f);
	contentView.frame = frame;
	[view addSubview:contentView];
	
	frame = CGRectMake(0.0f, frame.size.height - 69.0f, frame.size.width, 44.0f);
	_toolBar = [[UIToolbar alloc] initWithFrame:frame];
	[view addSubview:_toolBar];
	
	self.view = view;
	[view release];
	[self.toolBar release];
}

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated {
	NSArray* items = nil;
	if ([viewController respondsToSelector:@selector(toolBarItems)]) {
		items = [viewController performSelector:@selector(toolBarItems)];
	}
	[_toolBar setItems:items animated:animated];
}

#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
    NSLog(@"Queried about %@", NSStringFromSelector(sel));
    return [super respondsToSelector:sel];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[self.contentViewController release];
	
    [super dealloc];
}


@end
