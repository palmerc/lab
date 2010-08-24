//
//  ChartView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 20.08.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "ChartView.h"


@implementation ChartView
@synthesize delegate;
@synthesize chart = _chart;
@synthesize periodSelectionControl = _periodSelectionControl;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		_chart = [[UIImageView alloc] initWithFrame:CGRectZero];
		[self addSubview:_chart];

		NSString *oneDay = NSLocalizedString(@"oneDay", @"1 day");
		NSString *oneMonth = NSLocalizedString(@"oneMonth", @"1 month");
		NSString *oneYear = NSLocalizedString(@"oneYear", @"1 year");
		
		NSArray *items = [NSArray arrayWithObjects:oneDay, oneMonth, oneYear, nil];
		_periodSelectionControl = [[UISegmentedControl alloc] initWithItems:items];
		_periodSelectionControl.segmentedControlStyle = UISegmentedControlStyleBar;
		_periodSelectionControl.selectedSegmentIndex = 0;
		
		UIBarButtonItem *barButtonItem = [[UIBarButtonItem alloc] initWithCustomView:_periodSelectionControl];
		[barButtonItem setStyle:UIBarButtonItemStyleBordered];
		
		_toolbar = [[UIToolbar alloc] initWithFrame:CGRectZero];
		_toolbar.hidden = YES;

		_toolbar.items = [NSArray arrayWithObject:barButtonItem];
		[barButtonItem release];
		
		[self addSubview:_toolbar];		
	}
    return self;
}


- (void)layoutSubviews {
	CGRect bounds = self.bounds;
	
	CGRect toolBarFrame = CGRectMake(0.0f, bounds.size.height - 44.0f, bounds.size.width, 44.0f);
	
	_toolbar.frame = toolBarFrame;
	_chart.frame = bounds;
	if (self.delegate && [self.delegate respondsToSelector:@selector(chartRequest)]) {
		[self.delegate chartRequest];
	}
}

- (void)dealloc {
	[_toolbar release];
	[_periodSelectionControl release];
	[_chart release];
		
    [super dealloc];
}


@end
