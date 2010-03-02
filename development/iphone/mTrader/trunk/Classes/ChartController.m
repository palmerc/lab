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
@synthesize managedObjectContext = _managedObjectContext;;
@synthesize symbol = _symbol;
@synthesize chart = _chart;
@synthesize toolBar = _toolBar;

#pragma mark -
#pragma mark Initialization

- (id)initWithSymbol:(Symbol *)symbol {
    if (self = [super init]) {
		self.symbol = symbol;
		
		_managedObjectContext = nil;
		_chart = nil;
		
		period = 0;
		globalY = 0.0;
	}
    return self;
}

- (void)viewDidLoad {
	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];
	
	self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight;
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.rightBarButtonItem = doneButton;
	[doneButton release];
	
	NSArray *items = [NSArray arrayWithObjects:@"1d", @"1w", @"1m", @"1y", nil];
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
	
	communicator.symbolsDelegate = self;
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

- (void)chartUpdate:(NSDictionary *)chartData {
	NSArray *feedTickerComponents = [[chartData objectForKey:@"feedTicker"] componentsSeparatedByString:@"/"];
	NSNumber *feedNumber = [NSNumber numberWithInteger:[[feedTickerComponents objectAtIndex:0] integerValue]];
	NSString *tickerSymbol = [feedTickerComponents objectAtIndex:1];
	
	Symbol *symbol = [self fetchSymbol:tickerSymbol withFeedNumber:feedNumber];
	
	Chart *chart = (Chart *)[NSEntityDescription insertNewObjectForEntityForName:@"Chart" inManagedObjectContext:self.managedObjectContext];
	chart.height = [chartData objectForKey:@"height"];
	chart.width = [chartData objectForKey:@"width"];
	chart.size = [chartData objectForKey:@"size"];
	chart.type = [chartData objectForKey:@"type"];
	NSData *data = [chartData objectForKey:@"data"];
	chart.data = data;
	symbol.chart = chart;
	
	[self updateChart];
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
			period = 7;
			break;
		case 2:
			period = 30;
			break;
		case 3:
			period = 365;
			break;
		default:
			break;
	}
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:period width:self.chart.bounds.size.width height:self.chart.bounds.size.height orientation:@"P"];
}

#pragma mark -
#pragma mark Core Data Lookups
- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber {
	NSEntityDescription *entityDescription = [NSEntityDescription entityForName:@"Symbol" inManagedObjectContext:self.managedObjectContext];
	NSFetchRequest *request = [[[NSFetchRequest alloc] init] autorelease];
	[request setEntity:entityDescription];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(feed.feedNumber=%@) AND (tickerSymbol=%@)", feedNumber, tickerSymbol];
	[request setPredicate:predicate];
	
	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"tickerSymbol" ascending:YES];
	[request setSortDescriptors:[NSArray arrayWithObject:sortDescriptor]];
	[sortDescriptor release];
	
	NSError *error = nil;
	NSArray *array = [self.managedObjectContext executeFetchRequest:request error:&error];
	if (array == nil)
	{
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
	}
	
	if ([array count] == 1) {
		return [array objectAtIndex:0];
	} else {
		return nil;
	}
}

#pragma mark -
#pragma mark Actions
- (void)done:(id)sender {
	[self.delegate chartControllerDidFinish:self];
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_symbol release];
	[_managedObjectContext release];
	
	[_chart release];
	
    [super dealloc];
}


@end
