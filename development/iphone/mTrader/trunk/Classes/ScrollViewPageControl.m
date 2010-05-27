//
//  ScrollViewPageControl.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 14.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "ScrollViewPageControl.h"


@implementation ScrollViewPageControl
@synthesize frame = _frame;
@synthesize numberOfPages = _numberOfPages;
@synthesize scrollView = _scrollView;
@synthesize pageControl = _pageControl;
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
	aView.autoresizesSubviews = YES;
	self.view = aView;
	[aView release];
}

- (void)viewDidLoad {
	CGRect scrollViewFrame, pageControlFrame;
	scrollViewFrame = pageControlFrame = self.view.bounds;
	scrollViewFrame.size.height = self.view.bounds.size.height;
	pageControlFrame.origin.y = scrollViewFrame.size.height - 250.0f;
	pageControlFrame.size.height = 20.0f;
	
	_scrollView = [[UIScrollView alloc] initWithFrame:scrollViewFrame];
	self.scrollView.backgroundColor = [UIColor clearColor];
	self.scrollView.clipsToBounds = YES;
	self.scrollView.scrollEnabled = YES;
	self.scrollView.pagingEnabled = YES;
	self.scrollView.showsHorizontalScrollIndicator = NO;
	self.scrollView.delegate = self;
	[self.view addSubview:self.scrollView];
	
	[self adjustScrollView];

	_pageControl = [[UIPageControl alloc] initWithFrame:pageControlFrame];
	self.pageControl.backgroundColor = [UIColor clearColor];
	self.pageControl.currentPage = 1;
	self.pageControl.numberOfPages = [_views count];
	[self.pageControl addTarget:self action:@selector(changePage:) forControlEvents:UIControlEventValueChanged];
	[self.view addSubview:self.pageControl];
}

- (void)viewDidUnload {
	self.scrollView = nil;
	self.pageControl = nil;
}	

- (void)setFrame:(CGRect)frame {
	_frame = frame;
	self.view.frame = frame;
}

- (void)adjustScrollView {
	CGSize contentSize = self.scrollView.contentSize;
	contentSize.height = self.frame.size.height - 20.0f;
	
	for (UIView *view in self.views) {
		CGRect viewFrame = view.frame;
		
		viewFrame.origin.x = contentSize.width;
		contentSize.width += viewFrame.size.width;
		view.frame = viewFrame;
		
		[self.scrollView addSubview:view];
	}
	
	self.scrollView.contentSize = contentSize;
}

- (void)scrollViewDidScroll:(UIScrollView *)sender {
	CGFloat pageWidth = self.scrollView.frame.size.width;
    int page = floor((self.scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
    self.pageControl.currentPage = page;	
}

- (void)changePage:(id)sender {
    int page = self.pageControl.currentPage;
	
	// update the scroll view to the appropriate page
    CGRect frame = self.scrollView.frame;
    frame.origin.x = frame.size.width * page;
    frame.origin.y = 0;
    [self.scrollView scrollRectToVisible:frame animated:YES];
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	NSLog(@"Queried about %@ in SymbolScrollView", NSStringFromSelector(sel));
//	return [super respondsToSelector:sel];
//}

- (void)dealloc {
	[_scrollView release];
	[_pageControl release];
	[_views release];
    [super dealloc];
}


@end
