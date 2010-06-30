//
//  NewsArticleController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@class NewsArticleView_Phone;

@class NewsArticle;

@interface NewsArticleController_Phone : UIViewController {
@private
	NewsArticle *_newsArticle;
	
	NewsArticleView_Phone *_newsArticleView;
}

@property (nonatomic, retain) NewsArticle *newsArticle;
@property (nonatomic, retain) NewsArticleView_Phone *newsArticleView;

@end
