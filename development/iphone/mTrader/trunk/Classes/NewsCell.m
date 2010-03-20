//
//  NewsCell.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NewsCell.h"
#import "NewsFeed.h"
#import "NewsArticle.h"

#pragma mark -
#pragma mark SubviewFrames category

@interface NewsCell (SubviewFrames)
- (CGRect)_feedLabelFrame;
- (CGRect)_headlineLabelFrame;
- (CGRect)_dateTimeLabelFrame;
@end

@implementation NewsCell
@synthesize newsArticle = _newsArticle;

#pragma mark -
#pragma mark Initialization

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
		_newsArticle = nil;
		
		headlineLabelFont = [[UIFont systemFontOfSize:14.0] retain];
		headlineLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[headlineLabel setBackgroundColor:[UIColor colorWithWhite:1.0 alpha:0.0]];
		[headlineLabel setFont:headlineLabelFont];
		[headlineLabel setTextAlignment:UITextAlignmentLeft];
		[headlineLabel setTextColor:[UIColor blackColor]];
		[headlineLabel setHighlightedTextColor:[UIColor blackColor]];
		[self.contentView addSubview:headlineLabel];		
		
		bottomLineLabelFont = [[UIFont systemFontOfSize:12.0] retain];
		dateTimeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[dateTimeLabel setBackgroundColor:[UIColor colorWithWhite:1.0 alpha:0.0]];
		[dateTimeLabel setFont:bottomLineLabelFont];
		[dateTimeLabel setTextAlignment:UITextAlignmentLeft];
		[dateTimeLabel setTextColor:[UIColor lightGrayColor]];
		[dateTimeLabel setHighlightedTextColor:[UIColor lightGrayColor]];
		[self.contentView addSubview:dateTimeLabel];
		
		feedLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[feedLabel setBackgroundColor:[UIColor colorWithWhite:1.0 alpha:0.0]];
		feedLabel.textAlignment = UITextAlignmentRight;
		[feedLabel setFont:bottomLineLabelFont];
		[feedLabel setTextColor:[UIColor lightGrayColor]];
		[feedLabel setHighlightedTextColor:[UIColor lightGrayColor]];
		[self.contentView addSubview:feedLabel];
    }
    return self;
}

#pragma mark -
#pragma mark Laying out subviews

- (void)layoutSubviews {
    [super layoutSubviews];
	
    [headlineLabel setFrame:[self _headlineLabelFrame]];
	[dateTimeLabel setFrame:[self _dateTimeLabelFrame]];
	[feedLabel setFrame:[self _feedLabelFrame]];
}

#define TEXT_LEFT_MARGIN    8.0
#define TEXT_RIGHT_MARGIN   8.0

- (CGRect)_headlineLabelFrame {
	CGRect bounds = [[UIScreen mainScreen] bounds];
	CGSize fontSize = [@"X" sizeWithFont:headlineLabelFont];

	CGFloat width = bounds.size.width - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN;
	CGFloat height = fontSize.height;
	
	return CGRectMake(TEXT_LEFT_MARGIN, 2.0, width, height);
}

- (CGRect)_dateTimeLabelFrame {
	CGRect bounds = [[UIScreen mainScreen] bounds];
	CGSize fontSize = [@"X" sizeWithFont:bottomLineLabelFont];

	CGFloat width = (bounds.size.width - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN) / 2;
	CGFloat height = fontSize.height;
	
	return CGRectMake(TEXT_LEFT_MARGIN, 24.0, width, height);
}

- (CGRect)_feedLabelFrame {
	CGRect bounds = [[UIScreen mainScreen] bounds];
	CGSize fontSize = [@"X" sizeWithFont:bottomLineLabelFont];

	CGFloat width = (bounds.size.width - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN) / 2;
	CGFloat height = fontSize.height;
	
	return CGRectMake(TEXT_LEFT_MARGIN + width, 24.0, width, height);
}

#pragma mark -
#pragma mark Overriden accessor methods

- (void)setNewsArticle:(NewsArticle *)newsArticle {
	_newsArticle = [newsArticle retain];
	
	feedLabel.text = newsArticle.newsFeed.mCode;
	
	NSString *flag = newsArticle.flag;
	if ([flag isEqualToString:@"F"]) {
		[headlineLabel setTextColor:[UIColor redColor]];
	} else if ([flag isEqualToString:@"U"]) {
		[headlineLabel setTextColor:[UIColor blueColor]];
	} else {
		[headlineLabel setTextColor:[UIColor blackColor]];
	}

	
	headlineLabel.text = newsArticle.headline;
	dateTimeLabel.text = [NSString stringWithFormat:@"%@ %@", newsArticle.date, newsArticle.time];
}
/*
#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in NewsCell", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
*/
#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[headlineLabelFont release];
	[bottomLineLabelFont release];
	[feedLabel release];
	[headlineLabel release];
	[dateTimeLabel release];
	[_newsArticle release];
    [super dealloc];
}


@end
