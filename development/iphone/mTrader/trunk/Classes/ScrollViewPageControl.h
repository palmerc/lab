//
//  ScrollViewPageControl.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 14.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ScrollViewPageControl : UIViewController <UIScrollViewDelegate> {
@private
	CGRect _frame;
	UIScrollView *_scrollView;
	UIPageControl *_pageControl;
	
	NSMutableArray *_views;
	
	NSUInteger _numberOfPages;
}

@property (assign) CGRect frame;
@property (assign) NSUInteger numberOfPages;
@property (nonatomic, retain) UIScrollView *scrollView;
@property (nonatomic, retain) UIPageControl *pageControl;
@property (nonatomic, retain) NSMutableArray *views;

- (void)pushView:(UIView *)view;
- (void)adjustScrollView;

@end
