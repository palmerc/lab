//
//  TradesModalController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "TradesModalController.h"

#import "DataController.h"

#import <QuartzCore/QuartzCore.h>
#import "mTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "PastTradesController.h"

@implementation TradesModalController
@synthesize delegate;
@synthesize tradesController = _tradesController;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize symbol = _symbol;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		
		_tradesController = [[PastTradesController alloc] initWithManagedObjectContext:managedObjectContext];
		[self.view addSubview:_tradesController.view];
		_symbol = nil;
	}
	return self;
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
	
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
}

- (void)done:(id)sender {
	[self.delegate tradesControllerDidFinish:self];
}

- (void)refresh:(id)sender {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", self.symbol.feed.feedNumber, self.symbol.tickerSymbol];
	[communicator tradesRequest:feedTicker];
}

#pragma mark -
#pragma mark Debugging methods

#if DEBUG
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in TradesModalController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

- (void)dealloc {
	[_managedObjectContext release];
	[_symbol release];
		
    [super dealloc];
}


@end

