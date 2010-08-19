//
//  LastChangeView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "ChartController.h"

@class Symbol;

@interface LastChangeView : UIView {
@private
	UIViewController *_viewController;
	
	Symbol *_symbol;
	
	UILabel *_time;
	UILabel *_last;
	UILabel *_lastChange;
	UILabel *_lastPercentChange;
	
	UIButton *_chart;
}

@property (nonatomic, retain) UIViewController *viewController;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UILabel *time;
@property (nonatomic, retain) UILabel *last;
@property (nonatomic, retain) UILabel *lastChange;
@property (nonatomic, retain) UILabel *lastPercentChange;
@property (nonatomic, retain) UIButton *chart;

- (void)updateSymbol;
- (void)updateChart;

@end
