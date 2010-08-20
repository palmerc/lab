//
//  ChartView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 20.08.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ChartView : UIView {
@private
	UIImageView *_chartView;
	UISegmentedControl *_periodSelectionControl;
	UIToolbar *_toolbar;
}

@property (nonatomic, readonly) UIImageView *chartView;
@property (readonly) UISegmentedControl *periodSelectionControl;

@end
