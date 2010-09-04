//
//  ScrollPageView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.09.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 1

#import "ScrollPageView.h"


@implementation ScrollPageView
@synthesize delegate;
@synthesize scrollView = _scrollView;
@synthesize pageControl = _pageControl;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {		
		_scrollView = [[UIScrollView alloc] initWithFrame:CGRectZero];
		//_scrollView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
		//_scrollView.autoresizesSubviews = YES;
		_scrollView.clipsToBounds = YES;
		_scrollView.scrollEnabled = YES;
		_scrollView.pagingEnabled = YES;
		_scrollView.bounces = YES;
		_scrollView.showsHorizontalScrollIndicator = NO;
		[self addSubview:_scrollView];
		
		_pageControl = [[UIPageControl alloc] initWithFrame:CGRectZero];
		_pageControl.backgroundColor = [UIColor darkGrayColor];
		_pageControl.numberOfPages = 0;
		_pageControl.currentPage = 0;
		_pageControl.opaque = YES;
		[self addSubview:_pageControl];
	}
    return self;
}

- (void)layoutSubviews {
	_scrollView.frame = CGRectMake(0.0f, 0.0f, self.bounds.size.width, self.bounds.size.height - 10.0f);
	_pageControl.frame = CGRectMake(0.0f, self.bounds.size.height - 10.0f, self.bounds.size.width, 10.0f);

	[self.delegate adjustScrollView];
}

#if DEBUG
//Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"ScrollPageView: %@", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif


- (void)dealloc {
	[_scrollView release];
	[_pageControl release];
	
    [super dealloc];
}


@end
