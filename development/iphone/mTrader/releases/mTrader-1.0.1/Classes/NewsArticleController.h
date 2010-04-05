//
//  NewsArticleController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@class NewsArticleView;
@class NewsArticle;

@interface NewsArticleController : UIViewController {
@private
	NewsArticle *_newsArticle;
	
	NewsArticleView *_newsArticleView;
}

@property (nonatomic, retain) NewsArticle *newsArticle;
@property (nonatomic, retain) NewsArticleView *newsArticleView;

@end
