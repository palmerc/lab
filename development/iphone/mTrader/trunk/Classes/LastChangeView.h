//
//  LastChangeView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangle.h"

@class Symbol;

@interface LastChangeView : RoundedRectangle {
@private
	Symbol *_symbol;
	
	UILabel *_time;
	UILabel *_last;
	UILabel *_lastChange;
	UILabel *_lastPercentChange;
	
	UIImageView *_chart;
	
	NSUInteger chartWidth;
	NSUInteger chartHeight;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UILabel *time;
@property (nonatomic, retain) UILabel *last;
@property (nonatomic, retain) UILabel *lastChange;
@property (nonatomic, retain) UILabel *lastPercentChange;
@property (nonatomic, retain) UIImageView *chart;

- (void)updateSymbol;
- (void)updateChart;

@end
