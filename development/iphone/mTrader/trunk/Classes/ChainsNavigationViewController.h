//
//  ChainsNavigationViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class ChainsTableViewController;

@interface ChainsNavigationViewController : UINavigationController {
@private
	UIViewController *_chainsTableViewController;
	UIToolbar *_toolBar;
}

@property (nonatomic, retain) UIViewController *chainsTableViewController;
@property (nonatomic, retain, readonly) UIToolbar *toolBar;

- (id)initWithContentViewController:(UIViewController *)rootViewController;

@end
