//
//  StockDetailController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "StockDetailController.h"
#import "SymbolsController.h"
#import "mTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "Chart.h"

@implementation StockDetailController
@synthesize symbol = _symbol;
@synthesize symbolsController = _symbolsController;
@synthesize communicator = _communicator;
@synthesize previousUpdateDelegate;
@synthesize scrollView;
@synthesize nameLabel;
@synthesize statusLabel;
@synthesize stockISINLabel;
@synthesize lastTradeLabel;
@synthesize lastTradeChangeLabel;
@synthesize lastTradePercentChangeLabel;
@synthesize lastTradeTimeLabel;
@synthesize lowLabel;
@synthesize highLabel;
@synthesize volumeLabel;
@synthesize openLabel;
@synthesize openChangeLabel;
@synthesize openPercentChangeLabel;
@synthesize previousCloseLabel;
@synthesize vwapLabel;
@synthesize bidPriceLabel;
@synthesize bidSizeLabel;
@synthesize askPriceLabel;
@synthesize askSizeLabel;
@synthesize countryLabel;
@synthesize currencyLabel;
@synthesize outstandingSharesLabel;
@synthesize marketCapitalizationLabel;
@synthesize buyLotLabel;
@synthesize butLotValueLabel;
@synthesize turnoverLabel;
@synthesize onVolumeLabel;
@synthesize onValueLabel;
@synthesize averageVolumeLabel;
@synthesize averageValueLabel;
@synthesize chart;
@synthesize chartActivity;


- (id)initWithSymbol:(Symbol *)symbol {
	self = [super initWithNibName:@"StockDetailView" bundle:nil];
	if (self != nil) {
		self.symbolsController = [SymbolsController sharedManager];
		self.communicator = [iTraderCommunicator sharedManager];
		
		self.symbol = symbol;
		
		NSInteger feedIndex = [self.symbolsController indexOfFeedWithFeedNumber:self.symbol.feedNumber];
		Feed *feed = [self.symbolsController.feeds objectAtIndex:feedIndex];
		self.title = [NSString stringWithFormat:@"%@:%@", feed.mCode, self.symbol.tickerSymbol];
		self.hidesBottomBarWhenPushed = YES;
		
		[self.view addSubview:self.scrollView];
		CGSize size;
		size.height = 920;
		size.width = 320;
		self.scrollView.contentSize = size;
		
		UIView *pageOneView = [self loadViewFromNibNamed:@"StockDetailPageOne"];
		CGRect frameOne = CGRectMake(0, 0, 320, 460);
		[pageOneView setFrame:frameOne];
		[self.scrollView addSubview:pageOneView];

		UIView *pageTwoView = [self loadViewFromNibNamed:@"StockDetailPageTwo"];
		CGRect frameTwo = CGRectMake(0, 461, 320, 460);
		[pageTwoView setFrame:frameTwo];
		[self.scrollView addSubview:pageTwoView];
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
	[self.communicator graphForFeedTicker:self.symbol.feedTicker period:period width:150 height:150 orientation:@"A"];
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
	self.nameLabel.text = self.symbol.name;
	self.statusLabel.text = self.symbol.status;
	self.stockISINLabel.text = self.symbol.isin;
	self.lastTradeLabel.text = self.symbol.lastTrade;
	self.lastTradeChangeLabel.text = self.symbol.lastTradeChange;
	self.lastTradePercentChangeLabel.text = [NSString stringWithFormat:@"%@%%", self.symbol.lastTradePercentChange];
	self.lastTradeTimeLabel.text = self.symbol.lastTradeTime;
	self.openChangeLabel.text = self.symbol.openChange;
	self.openPercentChangeLabel.text = [NSString stringWithFormat:@"%@%%", self.symbol.openPercentChange];
	self.lowLabel.text = self.symbol.low;
	self.highLabel.text = self.symbol.high;
	self.volumeLabel.text = self.symbol.volume;
	self.openLabel.text = self.symbol.open;
	self.previousCloseLabel.text = self.symbol.previousClose;
	self.vwapLabel.text = self.symbol.VWAP;
	self.bidPriceLabel.text = self.symbol.bidPrice;
	self.bidSizeLabel.text = self.symbol.bidSize;
	self.askPriceLabel.text = self.symbol.askPrice;
	self.askSizeLabel.text = self.symbol.askSize;
	self.countryLabel.text = self.symbol.country;
	self.currencyLabel.text = self.symbol.currency;
	self.outstandingSharesLabel.text = self.symbol.outstandingShares;
	self.marketCapitalizationLabel.text = self.symbol.marketCapitalization;
	self.buyLotLabel.text = self.symbol.buyLot;
	self.butLotValueLabel.text = self.symbol.buyLotValue;
	self.turnoverLabel.text = self.symbol.turnover;
	self.onVolumeLabel.text = self.symbol.onVolume;
	self.onValueLabel.text = self.symbol.onValue;
	self.averageVolumeLabel.text = self.symbol.averageVolume;
	self.averageValueLabel.text = self.symbol.averageValue;
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
	[chartActivity startAnimating];
	chartActivity.hidden = NO;
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
	[self.communicator graphForFeedTicker:self.symbol.feedTicker period:period width:150 height:150 orientation:@"A"];
}

- (void)staticUpdated:(NSString *)feedTicker {
	[chartActivity startAnimating];
	chartActivity.hidden = NO;
	[self setValues];
	
	UIImage *image = [UIImage imageWithData:[self.symbol.chart image]];
	
	if (image != nil) {
		chartActivity.hidden = YES;
		[chartActivity stopAnimating];
		[self.chart setImage:image forState:UIControlStateNormal];
	}
}

@end
