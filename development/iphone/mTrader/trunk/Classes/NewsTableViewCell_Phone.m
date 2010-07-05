//
//  NewsTableViewCell_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "NewsTableViewCell_Phone.h"
#import "NewsFeed.h"
#import "NewsArticle.h"

#pragma mark -
#pragma mark SubviewFrames category

@interface NewsTableViewCell_Phone (SubviewFrames)
- (CGRect)_feedLabelFrame;
- (CGRect)_headlineLabelFrame;
- (CGRect)_dateTimeLabelFrame;
@end

@implementation NewsTableViewCell_Phone
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
		[dateTimeLabel setTextAlignment:UITextAlignmentRight];
		[dateTimeLabel setTextColor:[UIColor darkGrayColor]];
		[dateTimeLabel setHighlightedTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:dateTimeLabel];
		
		feedLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[feedLabel setBackgroundColor:[UIColor colorWithWhite:1.0 alpha:0.0]];
		feedLabel.textAlignment = UITextAlignmentLeft;
		[feedLabel setFont:bottomLineLabelFont];
		[feedLabel setTextColor:[UIColor darkGrayColor]];
		[feedLabel setHighlightedTextColor:[UIColor darkGrayColor]];
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

- (CGRect)_feedLabelFrame {
	CGSize fontSize = [feedLabel.text sizeWithFont:bottomLineLabelFont];

	CGFloat width = fontSize.width;
	CGFloat height = fontSize.height;
	
	return CGRectMake(TEXT_LEFT_MARGIN, 24.0, width, height);
}

- (CGRect)_dateTimeLabelFrame {
	CGSize fontSize = [dateTimeLabel.text sizeWithFont:bottomLineLabelFont];

	CGFloat width = fontSize.width;
	CGFloat height = fontSize.height;
	
	return CGRectMake(self.frame.size.width - width - TEXT_RIGHT_MARGIN, 24.0, width, height);
}

#pragma mark -
#pragma mark Overriden accessor methods

- (void)setNewsArticle:(NewsArticle *)newsArticle {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateStyle:NSDateFormatterShortStyle];
		[dateFormatter setTimeStyle:NSDateFormatterShortStyle];
	}
	
	_newsArticle = [newsArticle retain];
	
	feedLabel.text = newsArticle.newsFeed.name;
	
	NSString *flag = newsArticle.flag;
	if ([flag isEqualToString:@"F"]) {
		[headlineLabel setTextColor:[UIColor redColor]];
	} else if ([flag isEqualToString:@"U"]) {
		[headlineLabel setTextColor:[UIColor blueColor]];
	} else {
		[headlineLabel setTextColor:[UIColor blackColor]];
	}

	
	headlineLabel.text = newsArticle.headline;
	dateTimeLabel.text = [dateFormatter stringFromDate:newsArticle.date];
}


#pragma mark -
#pragma mark Debugging methods

#if DEBUG
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in NewsCell", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif
 
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
