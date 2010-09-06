//
//  NewsArticleController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "NewsArticleController_Phone.h"

#import "NSString+CleanStringAdditions.h"

#import "NewsFeed.h"
#import "NewsArticle.h"

@implementation NewsArticleController_Phone
@synthesize newsArticle = _newsArticle;

- (id)init {
	self = [super init];
	if (self != nil) {
		_newsArticle = nil;
		_webView = nil;
	}
	return self;
}

- (void)loadView {
	CGRect frame = [[UIScreen mainScreen] applicationFrame];
	_webView = [[UIWebView alloc] initWithFrame:frame];
	_webView.dataDetectorTypes = UIDataDetectorTypeLink;
	_webView.contentMode = UIViewContentModeScaleAspectFit; 
	_webView.scalesPageToFit = YES;
	_webView.delegate = self;
	self.view = _webView;
}

- (void)updateNewsArticle {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateStyle:NSDateFormatterShortStyle];
		[dateFormatter setTimeStyle:NSDateFormatterShortStyle];
	}
	
	self.title = _newsArticle.newsFeed.mCode;
	
	NSString *css = [NSString stringWithFormat:@"<style type=\"text/css\"> \
					 body {font-size:100%;} \
					 p {font-size:0.4em;} \
					 h1 {font-family:sans-serif; font-weight:bold; font-size:0.5em; margin-bottom:0;} \
					 h2 {font-family:sans-serif; font-weight:lighter; font-size:0.4em; margin-top:0;} \
					 div {clear:both;} \
					 div#feedDate {color:#8b8989;} \
					 div#feedDate h2#feed {float:left;} \
					 div#feedDate h2#date {float:right;} \
					 </style>"];
	NSString *date = [NSString stringWithFormat:@"<h2 id=\"date\">%@</h2>", [dateFormatter stringFromDate:_newsArticle.date]];
	NSString *feed = [NSString stringWithFormat:@"<h2 id=\"feed\">%@</h2>", _newsArticle.newsFeed.name];
	NSString *flag = _newsArticle.flag;
	
	NSString *color = nil;
	if ([flag isEqualToString:@"F"]) {
		color = @"style=\"color:red\"";
	} else if ([flag isEqualToString:@"U"]) {
		color = @"style=\"color:blue\"";
	}
	NSString *headline = [NSString stringWithFormat:@"<h1 %@>%@</h1>", color, _newsArticle.headline];
	NSString *bodyText = [[_newsArticle.body stringByReplacingOccurrencesOfString:@"||" withString:@"</p><p>"] sansWhitespace];
	NSString *body = [NSString stringWithFormat:@"<p>%@</p>", bodyText];
	
	NSString *html = [NSString stringWithFormat:@"<html><head>%@</head><body><div id=\"headline\">%@</div><div id=\"feedDate\">%@%@</div><div id=\"bodyText\">%@</div></body></html>", css, headline, feed, date, body];
	
	[_webView loadHTMLString:html baseURL:nil];
	
	[_newsArticle removeObserver:self forKeyPath:@"body"];
}

#pragma mark -
#pragma mark Delegation

- (void)setNewsArticle:(NewsArticle *)newsArticle {
	_newsArticle = [newsArticle retain];
	NSString *feedArticle = [NSString stringWithFormat:@"%@/%@", _newsArticle.newsFeed.feedNumber, _newsArticle.articleNumber];
	[[mTraderCommunicator sharedManager] newsItemRequest:feedArticle];
	
	[_newsArticle addObserver:self forKeyPath:@"body" options:NSKeyValueObservingOptionNew context:nil];
}


#pragma mark -
#pragma mark Debugging methods

#if DEBUG
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in NewsItemController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"body"]) {
		[self updateNewsArticle];
	}
}

#pragma mark -
#pragma mark UIWebView delegate methods
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
	if (navigationType == UIWebViewNavigationTypeLinkClicked) {
		NSURL *url = [request URL];
		[[UIApplication sharedApplication] openURL:url];
		return NO;
	}
	
	return YES;
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_newsArticle release];
	[_webView release];
    [super dealloc];
}


@end
