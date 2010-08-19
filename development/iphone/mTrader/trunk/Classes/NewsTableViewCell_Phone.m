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
		_headlineFont = [[UIFont boldSystemFontOfSize:14.0f] retain];
		_bottomlineFont = [[UIFont systemFontOfSize:12.0f] retain];
		
		_headlineLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_headlineLabel setLineBreakMode:UILineBreakModeWordWrap];
		[_headlineLabel setNumberOfLines:0];
		[_headlineLabel setFont:_headlineFont];
		[_headlineLabel setTextAlignment:UITextAlignmentLeft];
		[_headlineLabel setTextColor:[UIColor blackColor]];
		[_headlineLabel setHighlightedTextColor:[UIColor blackColor]];
		[self.contentView addSubview:_headlineLabel];		
		
		_dateTimeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_dateTimeLabel setFont:_bottomlineFont];
		[_dateTimeLabel setTextAlignment:UITextAlignmentRight];
		[_dateTimeLabel setTextColor:[UIColor darkGrayColor]];
		[_dateTimeLabel setHighlightedTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:_dateTimeLabel];

		_feedLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_feedLabel.textAlignment = UITextAlignmentLeft;
		[_feedLabel setTextColor:[UIColor darkGrayColor]];
		[_feedLabel setHighlightedTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:_feedLabel];
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
	
	if (_newsArticle == newsArticle) {
		return;
	}
	[_newsArticle release];
	_newsArticle = [newsArticle retain];
		
	NSString *flag = newsArticle.flag;
	if ([flag isEqualToString:@"F"]) {
		[_headlineLabel setTextColor:[UIColor redColor]];
	} else if ([flag isEqualToString:@"U"]) {
		[_headlineLabel setTextColor:[UIColor blueColor]];
	} else {
		[_headlineLabel setTextColor:[UIColor blackColor]];
	}
	
	NSString *headlineString = _newsArticle.headline;
	_headlineLabel.text = headlineString;
	_headlineLabel.font = _headlineFont;
	
	NSString *newsFeedString = _newsArticle.newsFeed.name;
	_feedLabel.text = newsFeedString;
	_feedLabel.font = _bottomlineFont;
	
	NSString *dateTimeString = [dateFormatter stringFromDate:_newsArticle.date];
	_dateTimeLabel.text = dateTimeString;
	_dateTimeLabel.font = _bottomlineFont;	

}

- (void)layoutSubviews {
	[super layoutSubviews];
	
	CGFloat windowWidth = self.bounds.size.width;
	CGSize constraint = CGSizeMake(windowWidth - (_contentMargin * 2.0f), 2000.0f);
	CGFloat x = floorf(_contentMargin / 2.0f);
	CGFloat y = x;
	
	CGSize headlineSize = [_headlineLabel.text sizeWithFont:_headlineFont constrainedToSize:constraint lineBreakMode:UILineBreakModeWordWrap];
	CGRect headlineFrame = CGRectMake(x, y, headlineSize.width, headlineSize.height);
	_headlineLabel.frame = headlineFrame;
	
	CGSize newsFeedSize = [_feedLabel.text sizeWithFont:_bottomlineFont];
	CGRect feedLabelFrame = CGRectMake(x, headlineSize.height + y, newsFeedSize.width, newsFeedSize.height);
	_feedLabel.frame = feedLabelFrame;

	
	CGSize dateTimeSize = [_dateTimeLabel.text sizeWithFont:_bottomlineFont];
	CGRect dateTimeFrame = CGRectMake(windowWidth - dateTimeSize.width - x, headlineSize.height + y, dateTimeSize.width, newsFeedSize.height);
	_dateTimeLabel.frame = dateTimeFrame;
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
	[_feedLabel release];
	[_headlineLabel release];
	[_dateTimeLabel release];
	[_newsArticle release];
    [super dealloc];
}

@end
