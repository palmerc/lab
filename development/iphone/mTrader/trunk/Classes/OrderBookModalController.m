//
//  OrderBookModalController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "OrderBookModalController.h"

#import "DataController.h"
#import "QFields.h"

#import "OrderBookController.h"

#import "Feed.h"
#import "Symbol.h"

@implementation OrderBookModalController
@synthesize delegate;
@synthesize symbol = _symbol;
@synthesize managedObjectContext = _managedObjectContext;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		
		
		orderBook = [[OrderBookController alloc] initWithManagedObjectContext:self.managedObjectContext];
		[self.view addSubview:orderBook.view];
		
	}
	return self;
}

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

- (void)viewDidLoad {
	[super viewDidLoad];

	UIFont *headerFont = [UIFont boldSystemFontOfSize:18.0];
	
	CGFloat maxWidth = self.view.frame.size.width;
	
	CGSize headerFontSize = [@"X" sizeWithFont:headerFont];
	
	CGFloat labelWidth = floorf(maxWidth / 4.0f);
	CGRect bidSizeLabelFrame = CGRectMake(0.0f, 0.0f, labelWidth, headerFontSize.height);
	CGRect bidValueLabelFrame = CGRectMake(0.0f + labelWidth, 0.0f, labelWidth, headerFontSize.height);
	CGRect askValueLabelFrame = CGRectMake(0.0f + labelWidth * 2.0f, 0.0f, labelWidth, headerFontSize.height);
	CGRect askSizeLabelFrame = CGRectMake(0.0f + labelWidth * 3.0f, 0.0f, labelWidth, headerFontSize.height);
	
	askSizeLabel = [[UILabel alloc] initWithFrame:askSizeLabelFrame];
	askSizeLabel.textAlignment = UITextAlignmentCenter;
	askSizeLabel.font = headerFont;
	askSizeLabel.text = @"A Size";
	
	askValueLabel = [[UILabel alloc] initWithFrame:askValueLabelFrame];
	askValueLabel.textAlignment = UITextAlignmentCenter;
	askValueLabel.font = headerFont;
	askValueLabel.text = @"A Price";
	
	bidSizeLabel = [[UILabel alloc] initWithFrame:bidSizeLabelFrame];
	bidSizeLabel.textAlignment = UITextAlignmentCenter;
	bidSizeLabel.font = headerFont;
	bidSizeLabel.text = @"B Size";
	
	bidValueLabel = [[UILabel alloc] initWithFrame:bidValueLabelFrame];
	bidValueLabel.textAlignment = UITextAlignmentCenter;
	bidValueLabel.font = headerFont;
	bidValueLabel.text = @"B Price";
	
	[self.view addSubview:askSizeLabel];
	[self.view addSubview:askValueLabel];
	[self.view addSubview:bidSizeLabel];
	[self.view addSubview:bidValueLabel];
		
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];

	self.title = [NSString stringWithFormat:@"%@ (%@)", self.symbol.tickerSymbol, self.symbol.feed.mCode];

	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	
	[[DataController sharedManager] deleteAllBidsAsks];
	
	QFields *qFields = [[QFields alloc] init];
	qFields.timeStamp = YES;
	qFields.lastTrade = YES;
	qFields.change = YES;
	qFields.changePercent = YES;
	qFields.open = YES;
	qFields.high = YES;
	qFields.low = YES;
	qFields.volume = YES;
	qFields.orderBook = YES;
	
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:feedTicker];	
}

- (void)setSymbol:(Symbol *)symbol {
	if (_symbol != nil) {
		[_symbol release];
	}
	_symbol = [symbol retain];
	
	orderBook.symbol = self.symbol;
}

- (void)done:(id)sender {
	[self.delegate orderBookModalControllerDidFinish:self];
}

- (void)dealloc {
	[orderBook release];
	[askSizeLabel release];
	[askValueLabel release];
	[bidSizeLabel release];
	[bidValueLabel release];
	
    [super dealloc];
}


@end
