//
//  SymbolNewsItemView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@interface SymbolNewsItemView : UIView <UIScrollViewDelegate> {
@private
	UIScrollView *scrollView;
	UILabel *_headlineLabel;
	UILabel *_feedLabel;
	UILabel *_dateTimeLabel;
	UILabel *_bodyLabel;
	
	NSString *_flags;
	
	UIFont *headlineLabelFont;
	UIFont *bottomLineLabelFont;
	UIFont *bodyLabelFont;
}

@property (nonatomic, retain) UILabel *headlineLabel;
@property (nonatomic, retain) UILabel *feedLabel;
@property (nonatomic, retain) UILabel *dateTimeLabel;
@property (nonatomic, retain) UILabel *bodyLabel;

@property (nonatomic, retain) NSString *flags;

@end
