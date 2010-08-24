//
//  ChartView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 20.08.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@protocol ChartRequestDelegate;

@interface ChartView : UIView {
@private
	id <ChartRequestDelegate> delegate;
	
	UIImageView *_chart;
	UISegmentedControl *_periodSelectionControl;
	UIToolbar *_toolbar;
}

@property (nonatomic, assign) id <ChartRequestDelegate> delegate;
@property (nonatomic, readonly) UIImageView *chart;
@property (readonly) UISegmentedControl *periodSelectionControl;

@end

@protocol ChartRequestDelegate <NSObject>
- (void)chartRequest;
@end;