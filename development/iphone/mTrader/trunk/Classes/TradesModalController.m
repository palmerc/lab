//
//  TradesModalController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesModalController.h"

#import "SymbolDataController.h"

#import <QuartzCore/QuartzCore.h>
#import "mTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "TradesController.h"

@implementation TradesModalController
@synthesize delegate;
@synthesize tradesController = _tradesController;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize symbol = _symbol;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		
		_tradesController = [[TradesController alloc] initWithManagedObjectContext:managedObjectContext];
		[self.view addSubview:_tradesController.view];
		_symbol = nil;
	}
	return self;
}

- (void)viewDidLoad {
	self.view.backgroundColor = [UIColor whiteColor];
	
	UIFont *headerFont = [UIFont boldSystemFontOfSize:18.0];
	
	CGSize headerFontSize = [@"X" sizeWithFont:headerFont];
	CGFloat fifthWidth = floorf(self.view.bounds.size.width / 3.0f);
	CGRect buyerSellerLabelFrame = CGRectMake(0.0f, 0.0f, fifthWidth, headerFontSize.height);
	CGRect sizeLabelFrame = CGRectMake(0.0f + fifthWidth, 0.0f, fifthWidth, headerFontSize.height);
	CGRect priceLabelFrame = CGRectMake(0.0f + fifthWidth * 2.0f, 0.0f, fifthWidth, headerFontSize.height);
	
	UILabel *priceLabel = [[UILabel alloc] initWithFrame:priceLabelFrame];
	priceLabel.textAlignment = UITextAlignmentRight;
	priceLabel.font = headerFont;
	priceLabel.text = @"Price";
	
	UILabel *sizeLabel = [[UILabel alloc] initWithFrame:sizeLabelFrame];
	sizeLabel.textAlignment = UITextAlignmentRight;
	sizeLabel.font = headerFont;
	sizeLabel.text = @"Volume";
	
	UILabel *buyerSellerLabel = [[UILabel alloc] initWithFrame:buyerSellerLabelFrame];
	buyerSellerLabel.textAlignment = UITextAlignmentLeft;
	buyerSellerLabel.font = headerFont;
	buyerSellerLabel.text = @"Buyer/Seller";
	
	[self.view addSubview:buyerSellerLabel];
	[self.view addSubview:priceLabel];
	[self.view addSubview:sizeLabel];
	[buyerSellerLabel release];
	[priceLabel release];
	[sizeLabel release];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
	
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
	
	[super viewDidLoad];
}

- (void)setSymbol:(Symbol *)symbol {	
	if (_symbol != nil) {
		[_symbol release];
	}
	_symbol = [symbol retain];
	_tradesController.symbol = symbol; 

	self.title = [NSString stringWithFormat:@"%@ (%@)", symbol.tickerSymbol, symbol.feed.mCode];

}

- (void)done:(id)sender {
	[self.delegate tradesControllerDidFinish:self];
}

- (void)refresh:(id)sender {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[communicator tradesRequest:feedTicker];
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in TradesModalController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}


- (void)dealloc {
	[_managedObjectContext release];
	[_symbol release];
		
    [super dealloc];
}


@end

