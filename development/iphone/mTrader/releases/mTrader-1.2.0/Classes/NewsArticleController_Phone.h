//
//  NewsArticleController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@class NewsArticle;

@interface NewsArticleController_Phone : UIViewController <UIWebViewDelegate> {
@private
	UIWebView *_webView;
	
	NewsArticle *_newsArticle;	
}

@property (nonatomic, retain) NewsArticle *newsArticle;

@end
