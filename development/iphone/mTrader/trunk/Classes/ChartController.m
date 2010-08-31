//
//  ChartController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "ChartController.h"

#import "ChartView.h"
#import <QuartzCore/QuartzCore.h>
#import "Feed.h"
#import "Symbol.h"
#import "Chart.h"

@implementation ChartController
@synthesize delegate;
@synthesize symbol = _symbol;
@synthesize period = _period;
@synthesize orientation = _orientation;
@synthesize modal = _modal;

#pragma mark -
#pragma mark Initialization

- (id)init {
    if (self = [super init]) {
		_symbol = nil;

		_chartView = nil;
		
		_period = 0;
		_orientation = @"H";
		_modal = NO;
	}
    return self;
}

- (void)loadView {
	_chartView = [[ChartView alloc] initWithFrame:CGRectZero];
	_chartView.delegate = self;
	_chartView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	_chartView.autoresizesSubviews = YES;
	_chartView.modal = _modal;
	[_chartView.periodSelectionControl addTarget:self action:@selector(periodSelectionChanged:) forControlEvents:UIControlEventValueChanged];
	
	self.view = _chartView;
}

- (void)setSymbol:(Symbol *)symbol {
	if (symbol == _symbol) {
		return;
	}

	[_symbol release];
	_symbol = [symbol retain];
	
	[_symbol addObserver:self forKeyPath:@"chart.data" options:NSKeyValueObservingOptionNew context:nil];
}

- (void)updateChart {
	NSData *data = _symbol.chart.data;
	UIImage *image = [UIImage imageWithData:data];
	_chartView.chart.image = image;
}

- (void)setPeriod:(NSUInteger)period {
	_period = period;
	
	[self chartRequest];
}

- (void)periodSelectionChanged:(id)sender {
	NSUInteger index = _chartView.periodSelectionControl.selectedSegmentIndex;
	
	switch (index) {
		case 0:
			_period = 0;
			break;
		case 1:
			_period = 30;
			break;
		case 2:
			_period = 365;
			break;
		default:
			break;
	}
	
	[self chartRequest];
}

#pragma mark -
#pragma mark Actions
- (void)chartRequest {
	CGRect bounds = self.view.bounds;
	
	if (bounds.size.height == 0 || bounds.size.width == 0) {
		return;
	}
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", _symbol.feed.feedNumber, _symbol.tickerSymbol];
	NSUInteger screenResolutionMultiplier = 1.0f;
	
	UIScreen *currentScreen = [UIScreen mainScreen];
	UIScreenMode *currentMode = [currentScreen currentMode];
	CGSize screenPixelSize = [currentMode size];
	
	if (screenPixelSize.height == 960.0f && screenPixelSize.width == 640.0f) {
		screenResolutionMultiplier = 2.0f;
	}
	
	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:_period width:bounds.size.width * screenResolutionMultiplier height:bounds.size.height * screenResolutionMultiplier orientation:_orientation];
}

#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"chart.data"]) {
		[self updateChart];
	}
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_symbol removeObserver:self forKeyPath:@"chart.data"];
	
	[_symbol release];	
	[_chartView release];
	
    [super dealloc];
}


@end
