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
	
	NSArray *_views;
	
	NSUInteger _numberOfPages;
}

@property (assign) CGRect frame;
@property (nonatomic, retain) NSArray *views;

- (void)adjustScrollView;

@end
