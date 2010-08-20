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

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
    if (self = [super init]) {
		_symbol = [symbol retain];
		
		_chartView = nil;
		
		_period = 0;
		_orientation = @"P";
	}
    return self;
}

- (void)loadView {	
	_chartView = [[ChartView alloc] initWithFrame:CGRectZero];
		
	self.view = (UIView *)_chartView;
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	[_symbol addObserver:self forKeyPath:@"chart.data" options:NSKeyValueObservingOptionNew context:nil];
	[self requestChartForPeriod:365];
}

- (void)updateChart {
	NSData *data = _symbol.chart.data;
	UIImage *image = [UIImage imageWithData:data];
	_chartView.chartView.image = image;		
}

#pragma mark -
#pragma mark Actions
- (void)requestChartForPeriod:(NSUInteger)period {
	CGRect bounds = self.view.bounds;
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", _symbol.feed.feedNumber, _symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:period width:bounds.size.width height:bounds.size.height orientation:_orientation];
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
	[_symbol release];	
	[_chartView release];
	
    [super dealloc];
}


@end
