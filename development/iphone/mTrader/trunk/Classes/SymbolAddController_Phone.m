//
//  SymbolAddController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolAddController_Phone.h"

#import "SymbolSearchController_Phone.h"

#import "Feed.h"
#import "QFields.h"
#import "mTraderCommunicator.h"

@implementation SymbolAddController_Phone
@synthesize delegate;
 
#pragma mark -
#pragma mark Initialization

- (id)initWithFrame:(CGRect)frame {
	self = [super init];
	if (self != nil) {
		_frame = frame;
	}
	return self;
}

#pragma mark -
#pragma mark UIViewController Methods
- (void)viewDidLoad {
	self.title = NSLocalizedString(@"addASymbol", @"Add a Symbol");
			
	UIBarButtonItem *cancel = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancel:)];
	self.navigationItem.leftBarButtonItem = cancel;
	[cancel release];
	
	CGRect searchFrame = self.view.bounds;
	UISearchBar *searchBar = [[UISearchBar alloc] initWithFrame:searchFrame];
	[searchBar sizeToFit];
	[self.view addSubview:searchBar];
	searchBar.autocapitalizationType = UITextAutocapitalizationTypeNone;
	searchBar.placeholder = NSLocalizedString(@"tickerSymbolOrName", @"Ticker Symbol or Name");
	
	SymbolSearchController_Phone *symbolSearchContoller = [[SymbolSearchController_Phone alloc] init];
	CGRect tableFrame = self.view.bounds;
	tableFrame.origin.y = searchBar.frame.size.height;
	tableFrame.size.height -= searchBar.frame.size.height;
	symbolSearchContoller.view.frame = tableFrame;
	DataController *dataController = [DataController sharedManager];
	dataController.searchDelegate = symbolSearchContoller;
	[self.view addSubview:symbolSearchContoller.tableView];
	
	UISearchDisplayController *searchDisplayController = [[UISearchDisplayController alloc] initWithSearchBar:searchBar contentsController:symbolSearchContoller];
	searchDisplayController.delegate = symbolSearchContoller;
	searchDisplayController.searchResultsDataSource = symbolSearchContoller;
	searchDisplayController.searchResultsDelegate = symbolSearchContoller;
	
	
	[searchBar release];		
}

- (void)viewWillAppear:(BOOL)animated {
	[self changeQFieldsStreaming];
}

#pragma mark -
#pragma mark mTrader Specific Methods

- (void)changeQFieldsStreaming {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	QFields *qFields = [[QFields alloc] init];
	
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:nil];
}

#pragma mark -
#pragma mark Actions

- (void)cancel:(id)sender {
	[self.delegate symbolAddControllerDidFinish:self didAddSymbol:nil];
}

#pragma mark -
#pragma mark Debugging methods

// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	 NSLog(@"Queried about %@", NSStringFromSelector(sel));
//	 return [super respondsToSelector:sel];
//}

#pragma mark -
#pragma mark Memory Management
- (void)dealloc {
	
	[super dealloc];
}

@end
