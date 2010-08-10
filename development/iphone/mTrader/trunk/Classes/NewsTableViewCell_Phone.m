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
@synthesize headlineFont = _headlineFont;
@synthesize bottomlineFont = _bottomlineFont;
@synthesize contentMargin = _contentMargin;

#pragma mark -
#pragma mark Initialization

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
		_newsArticle = nil;
		_contentMargin = 0.0f;
		
		headlineLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[headlineLabel setLineBreakMode:UILineBreakModeWordWrap];
		[headlineLabel setNumberOfLines:0];
		[headlineLabel setFont:_headlineFont];
		[headlineLabel setTextAlignment:UITextAlignmentLeft];
		[headlineLabel setTextColor:[UIColor blackColor]];
		[headlineLabel setHighlightedTextColor:[UIColor blackColor]];
		[self.contentView addSubview:headlineLabel];		
		
		dateTimeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[dateTimeLabel setFont:_bottomlineFont];
		[dateTimeLabel setTextAlignment:UITextAlignmentRight];
		[dateTimeLabel setTextColor:[UIColor darkGrayColor]];
		[dateTimeLabel setHighlightedTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:dateTimeLabel];

		feedLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		feedLabel.textAlignment = UITextAlignmentLeft;
		[feedLabel setTextColor:[UIColor darkGrayColor]];
		[feedLabel setHighlightedTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:feedLabel];
    }
    return self;
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
		
	NSString *flag = newsArticle.flag;
	if ([flag isEqualToString:@"F"]) {
		[headlineLabel setTextColor:[UIColor redColor]];
	} else if ([flag isEqualToString:@"U"]) {
		[headlineLabel setTextColor:[UIColor blueColor]];
	} else {
		[headlineLabel setTextColor:[UIColor blackColor]];
	}

	CGFloat windowWidth = self.frame.size.width;
	CGSize constraint = CGSizeMake(windowWidth - (_contentMargin * 2.0f), 2000.0f);
	CGFloat x = floorf(_contentMargin / 2.0f);
	CGFloat y = x;
	
	NSString *headlineString = newsArticle.headline;
	CGSize headlineSize = [headlineString sizeWithFont:_headlineFont constrainedToSize:constraint lineBreakMode:UILineBreakModeWordWrap];
	CGRect headlineFrame = CGRectMake(x, y, headlineSize.width, headlineSize.height);
	headlineLabel.frame = headlineFrame;
	headlineLabel.text = headlineString;
	headlineLabel.font = _headlineFont;
	
	NSString *newsFeedString = newsArticle.newsFeed.name;
	CGSize newsFeedSize = [newsFeedString sizeWithFont:_bottomlineFont];
	CGRect feedLabelFrame = CGRectMake(x, headlineSize.height + y, newsFeedSize.width, newsFeedSize.height);
	feedLabel.frame = feedLabelFrame;
	feedLabel.text = newsFeedString;
	feedLabel.font = _bottomlineFont;
	
	NSString *dateTimeString = [dateFormatter stringFromDate:newsArticle.date];
	CGSize dateTimeSize = [dateTimeString sizeWithFont:_bottomlineFont];
	CGRect dateTimeFrame = CGRectMake(windowWidth - dateTimeSize.width - x, headlineSize.height + y, dateTimeSize.width, newsFeedSize.height);
	dateTimeLabel.frame = dateTimeFrame;
	dateTimeLabel.text = dateTimeString;
	dateTimeLabel.font = _bottomlineFont;
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
	[_headlineFont release];
	[_bottomlineFont release];
	[feedLabel release];
	[headlineLabel release];
	[dateTimeLabel release];
	[_newsArticle release];
    [super dealloc];
}

@end
