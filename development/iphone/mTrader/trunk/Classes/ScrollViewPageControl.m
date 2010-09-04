	//
//  ScrollViewPageControl.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 14.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 1

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
	ScrollPageView *scrollPageView = [[ScrollPageView alloc] initWithFrame:CGRectZero];
	scrollPageView.delegate = self;
	
	_scrollView = [scrollPageView.scrollView retain];
	_scrollView.delegate = self;
	
	_pageControl = [scrollPageView.pageControl retain];
	[_pageControl addTarget:self action:@selector(changePage:) forControlEvents:UIControlEventValueChanged];
	
	self.view = scrollPageView;
	[scrollPageView release];
}

- (void)adjustScrollView {
	CGFloat height = _scrollView.bounds.size.height;
	
	CGSize contentSize = CGSizeZero;
	contentSize.height = height;
	
	for (UIView *aView in _views) {
		NSLog(@"View in adjustScrollView: %f %f %f %f", aView.frame.origin.x, aView.frame.origin.y, aView.frame.size.width, aView.frame.size.height);

		CGRect viewBounds = aView.bounds;
		
		viewBounds.origin.x = contentSize.width;
		contentSize.width += viewBounds.size.width;
		aView.frame = viewBounds;
	}
	_scrollView.contentSize = contentSize;
	NSLog(@"Scroll View ContentSize: %f %f", contentSize.width, contentSize.height);

}

- (void)setViews:(NSArray *)views {
	if (views == _views) {
		return;
	}
	[_views release];
	_views = [views retain];
	
	_numberOfPages = 0;

	for (UIView *aView in views) {
		NSLog(@"View added to scrollView: %f %f %f %f", aView.frame.origin.x, aView.frame.origin.y, aView.frame.size.width, aView.frame.size.height);
		[_scrollView addSubview:aView];
		_numberOfPages++;
	}
	
	_pageControl.numberOfPages = _numberOfPages;
}

- (void)scrollViewDidScroll:(UIScrollView *)sender {
	CGFloat pageWidth = _scrollView.bounds.size.width;
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
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"ScrollViewPageControl: %@", NSStringFromSelector(sel));
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
