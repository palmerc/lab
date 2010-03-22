//
//  ChartController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "ChartController.h"

#import <QuartzCore/QuartzCore.h>
#import "Feed.h"
#import "Symbol.h"
#import "Chart.h"

@implementation ChartController
@synthesize delegate;
@synthesize symbol = _symbol;
@synthesize chart = _chart;
@synthesize toolBar = _toolBar;

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
    if (self = [super init]) {
		self.symbol = symbol;
		[self.symbol addObserver:self forKeyPath:@"chart.data" options:NSKeyValueObservingOptionNew context:nil];
		
		_chart = nil;
		
		period = 0;
		globalY = 0.0;
	}
    return self;
}

- (void)viewDidLoad {	
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
	self.view.backgroundColor = [UIColor whiteColor];
	self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight;
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
	
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
	
	NSArray *items = [NSArray arrayWithObjects:@"1 day", @"1 month", @"1 year", nil];
	UISegmentedControl *chartPeriodSelector = [[UISegmentedControl alloc] initWithItems:items];
	chartPeriodSelector.segmentedControlStyle = UISegmentedControlStyleBar;
	chartPeriodSelector.selectedSegmentIndex = 0;
	[chartPeriodSelector addTarget:self action:@selector(chartPeriodSelected:) forControlEvents:UIControlEventValueChanged];
	
	UIBarButtonItem *barButtonItem = [[UIBarButtonItem alloc] initWithCustomView:chartPeriodSelector];
	[barButtonItem setStyle:UIBarButtonItemStyleBordered];
	
	CGRect mainFrame = [[UIScreen mainScreen] bounds];
	CGRect viewFrame = self.view.bounds;
	CGRect toolFrame = CGRectMake(0.0f, mainFrame.size.height - 44.0f, viewFrame.size.width, 44.0f);
	_toolBar = [[UIToolbar alloc] initWithFrame:toolFrame];
	
	self.toolBar.items = [NSArray arrayWithObject:barButtonItem];
	[self.navigationController.view addSubview:self.toolBar];
	
	CGRect chartFrame = CGRectMake(0.0f, viewFrame.origin.y, viewFrame.size.width, viewFrame.size.height - 44.0f * 2);
	NSLog(@"%f %f %f %f", chartFrame.origin.x, chartFrame.origin.y, chartFrame.size.width, chartFrame.size.height);
	_chart = [[UIImageView alloc] initWithFrame:chartFrame];
	[self.view addSubview:self.chart];
	
	[super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[communicator graphForFeedTicker:feedTicker period:period width:self.chart.bounds.size.width height:self.chart.bounds.size.height orientation:@"P"];
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

- (void)updateChart {
	Chart *chartChart = self.symbol.chart;
	NSData *data = chartChart.data;
	UIImage *image = [UIImage imageWithData:data];
	self.chart.image = image;		
}

#pragma mark -
#pragma mark Actions
- (void)chartPeriodSelected:(id)sender {
	NSLog(@"%@", sender);
	UISegmentedControl *control = sender;
	NSInteger index = control.selectedSegmentIndex;
	switch (index) {
		case 0:
			period = 0;
			break;
		case 1:
			period = 30;
			break;
		case 2:
			period = 365;
			break;
		default:
			break;
	}
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:period width:self.chart.bounds.size.width height:self.chart.bounds.size.height orientation:@"P"];
}

#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"chart.data"]) {
		[self updateChart];
	}
}

#pragma mark -
#pragma mark Actions
- (void)done:(id)sender {
	[self.delegate chartControllerDidFinish:self];
}

- (void)refresh:(id)sender {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	
	[communicator graphForFeedTicker:feedTicker period:period width:self.chart.bounds.size.width height:self.chart.bounds.size.height orientation:@"P"];
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[self.symbol removeObserver:self forKeyPath:@"chart.data"];
	[_symbol release];	
	[_chart release];
	
    [super dealloc];
}


@end
