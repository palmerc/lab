//
//  NewsTableViewCell_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@class NewsArticle;

@interface NewsTableViewCell_Phone : UITableViewCell {
@private
	NewsArticle *_newsArticle;
	
	UILabel *_feedLabel;
	UILabel *_headlineLabel;
	UILabel *_dateTimeLabel;
	
	UIFont *_headlineFont;
	UIFont *_bottomlineFont;

	CGFloat _contentMargin;
}

@property (nonatomic, retain) NewsArticle *newsArticle;
@property (nonatomic, retain) UIFont *headlineFont;
@property (nonatomic, retain) UIFont *bottomlineFont;
@property (nonatomic) CGFloat contentMargin;

@end
