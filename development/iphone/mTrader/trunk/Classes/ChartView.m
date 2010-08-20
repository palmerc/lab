//
//  ChartView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 20.08.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "ChartView.h"


@implementation ChartView
@synthesize chartView = _chartView;
@synthesize periodSelectionControl = _periodSelectionControl;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		self.backgroundColor = [UIColor blueColor];
		
		_chartView = [[UIImageView alloc] initWithFrame:CGRectZero];
		[self addSubview:_chartView];
		
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
		[_toolbar release];
		
	}
    return self;
}


- (void)layoutSubviews {
	CGRect bounds = self.bounds;
	
	CGSize imageSize = _chartView.image.size;
	CGRect toolBarFrame = CGRectMake(0.0f, bounds.size.height - 44.0f, bounds.size.width, 44.0f);
	
	_chartView.frame = CGRectMake(0.0f, 0.0f, imageSize.width, imageSize.height);
	_toolbar.frame = toolBarFrame;
}

- (void)dealloc {
	[_toolbar release];
	[_periodSelectionControl release];
	[_chartView release];
		
    [super dealloc];
}


@end
