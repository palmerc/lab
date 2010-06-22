//
//  NewsCell.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@class NewsArticle;

@interface NewsCell : UITableViewCell {
@private
	NewsArticle *_newsArticle;
	
	UILabel *feedLabel;
	UILabel *headlineLabel;
	UILabel *dateTimeLabel;
	
	UIFont *headlineLabelFont;
	UIFont *bottomLineLabelFont;
}

@property (nonatomic, retain) NewsArticle *newsArticle;

@end
