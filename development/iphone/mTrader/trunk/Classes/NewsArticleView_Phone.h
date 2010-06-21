//
//  NewsArticleView_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@interface NewsArticleView_Phone : UIView <UIScrollViewDelegate> {
@private
	UIScrollView *_scrollView;
	UILabel *_headlineLabel;
	UILabel *_feedLabel;
	UILabel *_dateTimeLabel;
	UILabel *_bodyLabel;
	
	NSString *_flags;
}

@property (nonatomic, retain) UIScrollView *scrollView;
@property (nonatomic, retain) UILabel *headlineLabel;
@property (nonatomic, retain) UILabel *feedLabel;
@property (nonatomic, retain) UILabel *dateTimeLabel;
@property (nonatomic, retain) UILabel *bodyLabel;

@property (nonatomic, retain) NSString *flags;

@end
