//
//  ScrollPageView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.09.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@protocol ScrollPageLayout;

@interface ScrollPageView : UIView {
@private
	id <ScrollPageLayout> delegate;
	
	UIScrollView *_scrollView;
	UIPageControl *_pageControl;

}
@property (assign) id <ScrollPageLayout> delegate;

@property (nonatomic,retain) UIScrollView *scrollView;
@property (nonatomic,retain) UIPageControl *pageControl;

@end

@protocol ScrollPageLayout
- (void)adjustScrollView;
@end