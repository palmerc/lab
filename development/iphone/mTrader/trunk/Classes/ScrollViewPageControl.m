//
//  ScrollViewPageControl.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 14.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "ScrollViewPageControl.h"


@implementation ScrollViewPageControl
@synthesize frame = _frame;
@synthesize views = _views;

- (id)initWithFrame:(CGRect)frame {
	self = [super init];
	if (self != nil) {
		_frame = frame;
		_scrollView = nil;
		_pageControl = nil;
		_numberOfPages = 0;
		_views = nil;
	}
	return self;
}

- (void)loadView {
	UIView *aView = [[UIView alloc] initWithFrame:_frame];
	
	CGRect scrollViewFrame = CGRectMake(0.0f, 0.0f, aView.bounds.size.width, aView.bounds.size.height - 10.0f);
	_scrollView = [[UIScrollView alloc] initWithFrame:scrollViewFrame];
	_scrollView.clipsToBounds = YES;
	_scrollView.scrollEnabled = YES;
	_scrollView.pagingEnabled = YES;
	_scrollView.showsHorizontalScrollIndicator = NO;
	_scrollView.delegate = self;
	[aView addSubview:_scrollView];
	
	CGRect pageControlFrame = CGRectMake(0.0f, 147.0f, aView.bounds.size.width, 10.0f);
	_pageControl = [[UIPageControl alloc] initWithFrame:pageControlFrame];
	_pageControl.backgroundColor = [UIColor darkGrayColor];
	_pageControl.numberOfPages = 0;
	_pageControl.currentPage = 0;
	_pageControl.opaque = YES;
	[_pageControl addTarget:self action:@selector(changePage:) forControlEvents:UIControlEventValueChanged];
	[aView addSubview:_pageControl];
	
	self.view = aView;
	[aView release];
	
	[self adjustScrollView];
}

- (void)adjustScrollView {
	_numberOfPages = 0;
	
	CGSize contentSize = _scrollView.contentSize;
	contentSize.height = self.frame.size.height - 20.0f;
	
	for (UIView *view in _views) {
		CGRect viewFrame = view.frame;
		
		viewFrame.origin.x = contentSize.width;
		contentSize.width += viewFrame.size.width;
		view.frame = viewFrame;
		
		[_scrollView addSubview:view];
		
		_numberOfPages++;
	}
	
	_pageControl.numberOfPages = _numberOfPages;
	_scrollView.contentSize = contentSize;
}

- (void)setViews:(NSArray *)views {
	if (_views == views) {
		return;
	}
	
	[_views release];
	_views = [views retain];
	
	[self adjustScrollView];
}

- (void)scrollViewDidScroll:(UIScrollView *)sender {
	CGFloat pageWidth = _scrollView.frame.size.width;
    int page = floor((_scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
    _pageControl.currentPage = page;	
}

- (void)changePage:(id)sender {
    int page = _pageControl.currentPage;
	
	// update the scroll view to the appropriate page
    CGRect frame = _scrollView.frame;
    frame.origin.x = frame.size.width * page;
    frame.origin.y = 0;
    [_scrollView scrollRectToVisible:frame animated:YES];
}

#pragma mark -
#pragma mark Debugging methods

#if DEBUG
 Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in SymbolScrollView", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

- (void)dealloc {
	[_scrollView release];
	[_pageControl release];
	[_views release];
    [super dealloc];
}


@end
