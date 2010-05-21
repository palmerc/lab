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
	scrollViewFrame.size.height = self.view.bounds.size.height - 10.0f;
	pageControlFrame.size.height = 10.0f;
	
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
	self.pageControl.currentPage = 0;
	self.pageControl.numberOfPages = 0;
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
	contentSize.height = self.frame.size.height - 10.0f;
	
	for (UIView *view in self.views) {
		CGRect viewFrame = view.frame;
		
		viewFrame.origin.x = contentSize.width;
		contentSize.width += viewFrame.size.width;
		view.frame = viewFrame;
		
		[self.scrollView addSubview:view];
	}
	
	self.scrollView.contentSize = contentSize;
}

- (void)pushView:(UIView *)view {
	if (_views == nil) {
		_views = [[NSMutableArray alloc] init];
	}
	
	[self.views addObject:view];
}

- (void)setNumberOfPages:(NSUInteger)numberOfPages {
	_numberOfPages = numberOfPages;
	self.pageControl.numberOfPages = numberOfPages;
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in SymbolScrollView", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}

- (void)dealloc {
	[_scrollView release];
	[_pageControl release];
	[_views release];
    [super dealloc];
}


@end
