//
//  ScrollViewPageControl.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 14.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "ScrollPageView.h"

@interface ScrollViewPageControl : UIViewController <UIScrollViewDelegate, ScrollPageLayout> {
@private
	UIScrollView *_scrollView;
	UIPageControl *_pageControl;
	
	NSArray *_views;
	
	NSUInteger _numberOfPages;
}

@property (nonatomic, retain) NSArray *views;

@end
