//
//  NewsArticleView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "NewsArticleView.h"


@implementation NewsArticleView
@synthesize scrollView = _scrollView;
@synthesize feedLabel = _feedLabel;
@synthesize dateTimeLabel = _dateTimeLabel;
@synthesize headlineLabel = _headlineLabel;
@synthesize bodyLabel = _bodyLabel;
@synthesize flags = _flags;

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {		
		[self setBackgroundColor:[UIColor whiteColor]];
		
		_scrollView = [[UIScrollView alloc] initWithFrame:frame];
		self.scrollView.delegate = self;
		self.scrollView.alwaysBounceVertical = YES;
		self.scrollView.bounces = YES;
		self.scrollView.minimumZoomScale = 1.0;
		self.scrollView.maximumZoomScale = 2.0;
		[self addSubview:self.scrollView];
		
		_headlineLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.headlineLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.headlineLabel.textColor = [UIColor blackColor];
		self.headlineLabel.numberOfLines = 0;
		
		_bodyLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.bodyLabel.font = [UIFont systemFontOfSize:12.0];
		self.bodyLabel.textColor = [UIColor blackColor];
		self.bodyLabel.numberOfLines = 0;
		
		_feedLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.feedLabel.font = [UIFont systemFontOfSize:12.0];
		self.feedLabel.textAlignment = UITextAlignmentRight;
		self.feedLabel.textColor = [UIColor lightGrayColor];
		
		_dateTimeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.dateTimeLabel.font = [UIFont systemFontOfSize:12.0];
		self.dateTimeLabel.textColor = [UIColor lightGrayColor];
		
		[self.scrollView addSubview:self.headlineLabel];
		[self.scrollView addSubview:self.bodyLabel];
		[self.scrollView addSubview:self.feedLabel];
		[self.scrollView addSubview:self.dateTimeLabel];
		
    }
    return self;
}

#define Y_PADDING 8.0f
#define TEXT_LEFT_MARGIN 8.0f
#define TEXT_RIGHT_MARGIN 8.0f

- (void)layoutSubviews {
	if ([self.flags isEqualToString:@"F"]) {
		self.headlineLabel.textColor = [UIColor redColor];
	} else if ([self.flags isEqualToString:@"U"]) {
		self.headlineLabel.textColor = [UIColor blueColor];
	} else {
		self.headlineLabel.textColor = [UIColor blackColor];
	}
	
	[self.headlineLabel sizeToFit];
	[self.bodyLabel sizeToFit];
	
	CGRect frame = self.frame;
	frame.size.height -= Y_PADDING;
	self.scrollView.frame = frame;
	
	CGFloat x = TEXT_LEFT_MARGIN;
	CGFloat y = Y_PADDING;
	CGFloat width = frame.size.width;
	CGFloat height = self.headlineLabel.frame.size.height;
	CGFloat sumY = height + Y_PADDING;
	
	self.headlineLabel.frame = CGRectMake(x, y, width - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN, height);
	
	[self.dateTimeLabel sizeToFit];
	[self.feedLabel sizeToFit];
	
	CGFloat halfWidth = width / 2.0f;
	
	self.dateTimeLabel.frame = CGRectMake(x, sumY, halfWidth - TEXT_LEFT_MARGIN, self.dateTimeLabel.frame.size.height);	
	self.feedLabel.frame = CGRectMake(halfWidth, sumY, halfWidth - TEXT_RIGHT_MARGIN, self.feedLabel.frame.size.height);
	sumY += height;
	
	height = self.bodyLabel.frame.size.height;
	self.bodyLabel.frame = CGRectMake(x, sumY, width - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN, height);
	sumY += height;
	
	self.scrollView.contentSize = CGSizeMake(frame.size.width, sumY);
}

//- (void)drawRect:(CGRect)rect {
//    // Drawing code
//}

#pragma mark -
#pragma mark UIScrollViewDelegate methods

//- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView {
//	return self.scrollView;
//}
//
//- (void)scrollViewDidEndZooming:(UIScrollView *)scrollView withView:(UIView *)view atScale:(float)scale {
//	self.headlineLabel.font = [UIFont boldSystemFontOfSize:14.0 * scale];
//	self.dateTimeLabel.font = [UIFont systemFontOfSize:12.0 * scale];
//	self.feedLabel.font = [UIFont systemFontOfSize:12.0 * scale];
//	self.bodyLabel.font = [UIFont systemFontOfSize:12.0 * scale];
//}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in NewsArticleView", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_headlineLabel release];
	[_feedLabel release];
	[_dateTimeLabel release];
	[_bodyLabel release];
	
	[_flags release];
	
	[_scrollView release];
	
    [super dealloc];
}


@end
