//
//  SymbolNewsItemView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolNewsItemView.h"


@implementation SymbolNewsItemView
@synthesize feedLabel = _feedLabel;
@synthesize dateTimeLabel = _dateTimeLabel;
@synthesize headlineLabel = _headlineLabel;
@synthesize bodyLabel = _bodyLabel;
@synthesize flags = _flags;

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {		
		headlineLabelFont = [[UIFont boldSystemFontOfSize:14.0] retain];
		bottomLineLabelFont = [[UIFont systemFontOfSize:12.0] retain];
		bodyLabelFont = [[UIFont systemFontOfSize:12.0] retain];
		
		[self setBackgroundColor:[UIColor whiteColor]];
		
		scrollView = [[UIScrollView alloc] initWithFrame:CGRectZero];
		scrollView.delegate = self;
		[self addSubview:scrollView];
		
		_headlineLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.headlineLabel.font = headlineLabelFont;
		self.headlineLabel.textColor = [UIColor blackColor];
		self.headlineLabel.numberOfLines = 0;
		
		_bodyLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.bodyLabel.font = bodyLabelFont;
		self.bodyLabel.textColor = [UIColor blackColor];
		self.bodyLabel.numberOfLines = 0;
		
		_feedLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.feedLabel.font = bottomLineLabelFont;
		self.feedLabel.textAlignment = UITextAlignmentRight;
		self.feedLabel.textColor = [UIColor lightGrayColor];
		
		_dateTimeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.dateTimeLabel.font = bottomLineLabelFont;
		self.dateTimeLabel.textColor = [UIColor lightGrayColor];

		[scrollView addSubview:self.headlineLabel];
		[scrollView addSubview:self.bodyLabel];
		[scrollView addSubview:self.feedLabel];
		[scrollView addSubview:self.dateTimeLabel];
		
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
	scrollView.frame = frame;
	
	CGFloat x = TEXT_LEFT_MARGIN;
	CGFloat y = Y_PADDING;
	CGFloat width = frame.size.width;
	CGFloat height = self.headlineLabel.frame.size.height;
	CGFloat sumY = height + Y_PADDING;
	
	self.headlineLabel.frame = CGRectMake(x, y, width - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN, height);
	
	CGFloat halfWidth = width / 2.0f;
	height = [@"X" sizeWithFont:bottomLineLabelFont].height;

	self.dateTimeLabel.frame = CGRectMake(x, sumY, halfWidth - TEXT_LEFT_MARGIN, height);	
	self.feedLabel.frame = CGRectMake(halfWidth, sumY, halfWidth - TEXT_RIGHT_MARGIN, height);
	sumY += height;
	
	height = self.bodyLabel.frame.size.height;
	self.bodyLabel.frame = CGRectMake(x, sumY, width - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN, height);
	sumY += height;
	
	scrollView.contentSize = CGSizeMake(frame.size.width, sumY);
}

- (void)drawRect:(CGRect)rect {
    // Drawing code
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in SymbolNewsItemView", NSStringFromSelector(sel));
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
	
	[headlineLabelFont release];
	[bottomLineLabelFont release];
	[bodyLabelFont release];
	
	[scrollView release];
	
    [super dealloc];
}


@end
