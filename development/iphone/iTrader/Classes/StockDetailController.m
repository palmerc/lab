//
//  StockDetailController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "StockDetailController.h"
#import "SymbolsController.h"
#import "iTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "Chart.h"

@implementation StockDetailController
@synthesize symbol = _symbol;
@synthesize symbolsController = _symbolsController;
@synthesize communicator = _communicator;
@synthesize previousUpdateDelegate;
@synthesize scrollView;
@synthesize stockNameLabel;
@synthesize stockISINLabel;
@synthesize exchangeLabel;
@synthesize lastChangeLabel;
@synthesize percentChangeLabel;
@synthesize tickerSymbolLabel;
@synthesize lowLabel;
@synthesize highLabel;
@synthesize volumeLabel;
@synthesize openLabel;
@synthesize graphImage;

- (id)initWithSymbol:(Symbol *)symbol {
	self = [super initWithNibName:@"StockDetailView" bundle:nil];
	if (self != nil) {
		self.symbolsController = [SymbolsController sharedManager];
		self.communicator = [iTraderCommunicator sharedManager];
		
		self.symbol = symbol;
		
		NSInteger feedIndex = [self.symbolsController indexOfFeedWithFeedNumber:self.symbol.feedNumber];
		Feed *feed = [self.symbolsController.feeds objectAtIndex:feedIndex];
		self.title = [NSString stringWithFormat:@"%@:%@", feed.code, self.symbol.tickerSymbol];
		self.hidesBottomBarWhenPushed = YES;
		
		[self.view addSubview:self.scrollView];
		CGSize size;
		size.height = 1200;
		size.width = 320;
		self.scrollView.contentSize = size;
		
		UIView *pageOneView = [self loadViewFromNibNamed:@"StockDetailPageOne"];
		CGRect frame = CGRectMake(0, 0, 320, 460);
		[pageOneView setFrame:frame];
		[self.scrollView addSubview:pageOneView];

		period = 0;		
	}
	
	return self;
}

- (UIView *)loadViewFromNibNamed:(NSString *)nibName {
	NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:nibName owner:self options:nil];
	
	for (id object in topLevelObjects) {
		if ([object isKindOfClass:[UIView class]]) {
			return object;
		}
	}
	return nil;
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

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
		
	self.previousUpdateDelegate = self.symbolsController.updateDelegate;
	self.symbolsController.updateDelegate = self;
	[self.communicator staticDataForFeedTicker:self.symbol.feedTicker];
	[self.communicator graphForFeedTicker:self.symbol.feedTicker period:period width:130 height:130 orientation:@"A"];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewWillDisappear:(BOOL)animated {
	self.symbolsController.updateDelegate = self.previousUpdateDelegate;
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

- (void)setValues {
	self.stockNameLabel.text = self.symbol.name;
	self.stockISINLabel.text = self.symbol.isin;
	self.exchangeLabel.text = self.symbol.feedNumber;
	self.lastChangeLabel.text = self.symbol.change;
	self.percentChangeLabel.text = [NSString stringWithFormat:@"%@%%", self.symbol.percentChange];
	self.tickerSymbolLabel.text = self.symbol.tickerSymbol;
	self.lowLabel.text = self.symbol.low;
	self.highLabel.text = self.symbol.high;
	self.volumeLabel.text = self.symbol.volume;
	self.openLabel.text = self.symbol.open;
}

- (void)symbolsAdded:(NSArray *)symbols {}
- (void)feedAdded:(Feed *)feed {}

- (void)symbolsUpdated:(NSArray *)quotes {
	for (NSString *feedTicker in quotes) {
		if ([feedTicker isEqual:self.symbol.feedTicker]) {
			[self setValues];
			[self.view setNeedsLayout];
		}
	}
}

- (IBAction)imageWasTapped:(id)sender {
	switch (period) {
		case 0:
			period = 30;
			break;
		case 30:
			period = 365;
			break;
		case 365:
			period = 0;
			break;
		default:
			break;
	}
	[self.communicator graphForFeedTicker:self.symbol.feedTicker period:period width:130 height:130 orientation:@"A"];
}

- (void)staticUpdated:(NSString *)feedTicker {
	[self setValues];
	[self.graphImage setImage:[self.symbol.chart image] forState:UIControlStateNormal];
}

@end
