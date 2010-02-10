//
//  UIToolbarController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 10.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@interface UIToolbarController : UIViewController <UINavigationControllerDelegate> {
@private
	UIViewController *_contentViewController;
	UIToolbar *_toolBar;
}

@property (nonatomic, retain, readonly) UIViewController *contentViewController;
@property (nonatomic, retain, readonly) UIToolbar *toolBar;

- (id)initWithContentViewController:(UIViewController *)contentViewController;

@end
