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
	ChainsTableViewController *_chainsTableViewController;
	UIToolbar *_toolBar;
}

@property (nonatomic, retain) ChainsTableViewController *chainsTableViewController;
@property (nonatomic, retain, readonly) UIToolbar *toolBar;

@end
