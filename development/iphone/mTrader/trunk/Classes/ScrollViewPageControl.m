//
//  ScrollViewPageControl.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 14.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "ScrollViewPageControl.h"

#import "ScrollPageView.h"

@implementation ScrollViewPageControl
@synthesize views = _views;

- (id)init {
	self = [super init];
	if (self != nil) {
		_scrollView = nil;
		_pageControl = nil;
		_numberOfPages = 0;
		_views = nil;
	}
	return self;
}

- (void)loadView {
	ScrollPageView *aView = [[ScrollPageView alloc] initWithFrame:CGRectZero];
	aView.delegate = self;
	
	_scrollView = [aView.scrollView retain];
	_scrollView.delegate = self;
	
	_pageControl = [aView.pageControl retain];
	[_pageControl addTarget:self action:@selector(changePage:) forControlEvents:UIControlEventValueChanged];
	
	self.view = aView;
	[aView release];
}

- (void)adjustScrollView {
	_numberOfPages = 0;
	
	CGSize contentSize = CGSizeZero;
	CGFloat height = _scrollView.bounds.size.height;
	contentSize.height = height;
	
	for (UIView *aView in _views) {
		CGRect viewBounds = aView.bounds;
		
		viewBounds.origin.x = contentSize.width;
		contentSize.width += viewBounds.size.width;
		aView.bounds = viewBounds;
		[_scrollView addSubview:aView];
		
		_numberOfPages++;
	}
	
	_pageControl.numberOfPages = _numberOfPages;
	_scrollView.contentSize = contentSize;
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
