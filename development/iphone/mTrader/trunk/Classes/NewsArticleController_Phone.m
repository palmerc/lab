//
//  NewsArticleController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NewsArticleController_Phone.h"

#import "NewsArticleView_Phone.h"

#import "NewsFeed.h"
#import "NewsArticle.h"
#import "StringHelpers.h"

@implementation NewsArticleController_Phone
@synthesize newsArticle = _newsArticle;
@synthesize newsArticleView = _newsArticleView;

- (id)init {
	self = [super init];
	if (self != nil) {
		_newsArticle = nil;
		_newsArticleView = nil;
	}
	return self;
}

- (void)loadView {
	CGRect frame = self.parentViewController.view.bounds;
	_newsArticleView = [[NewsArticleView_Phone alloc] initWithFrame:frame];
	self.view = _newsArticleView;
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

- (void)updateNewsArticle {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateStyle:NSDateFormatterShortStyle];
		[dateFormatter setTimeStyle:NSDateFormatterShortStyle];
	}
	
	self.title = self.newsArticle.newsFeed.mCode;
	self.newsArticleView.dateTimeLabel.text = [dateFormatter stringFromDate:self.newsArticle.date];
	self.newsArticleView.feedLabel.text = self.newsArticle.newsFeed.name;
	self.newsArticleView.flags = self.newsArticle.flag;
	self.newsArticleView.headlineLabel.text = self.newsArticle.headline;
	
	NSString *body = [self.newsArticle.body stringByReplacingOccurrencesOfString:@"||" withString:@"\n"];
	body = [StringHelpers cleanString:body];
	self.newsArticleView.bodyLabel.text = body;
	
	[self.newsArticleView layoutSubviews];
	
	[self.newsArticle removeObserver:self forKeyPath:@"body"];
}

#pragma mark -
#pragma mark Delegation

- (void)setNewsArticle:(NewsArticle *)newsArticle {
	_newsArticle = [newsArticle retain];
	NSString *feedArticle = [NSString stringWithFormat:@"%@/%@", self.newsArticle.newsFeed.feedNumber, self.newsArticle.articleNumber];
	[[mTraderCommunicator sharedManager] newsItemRequest:feedArticle];
	
	[self.newsArticle addObserver:self forKeyPath:@"body" options:NSKeyValueObservingOptionNew context:nil];
}
/*
#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in NewsItemController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
*/

#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"body"]) {
		[self updateNewsArticle];
	}
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {

	[_newsArticle release];
	[_newsArticleView release];
    [super dealloc];
}


@end
