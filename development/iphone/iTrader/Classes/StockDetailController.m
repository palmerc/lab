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

@implementation StockDetailController
@synthesize symbol = _symbol;
@synthesize symbolsController = _symbolsController;
@synthesize communicator = _communicator;
@synthesize previousUpdateDelegate;
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
		self.symbol = symbol;
		
		_symbolsController = [SymbolsController sharedManager];
		NSInteger feedIndex = [_symbolsController indexOfFeedWithFeedNumber:self.symbol.feedNumber];
		Feed *feed = [_symbolsController.feeds objectAtIndex:feedIndex];
		self.title = [NSString stringWithFormat:@"%@:%@", feed.code, self.symbol.tickerSymbol];
		
		_communicator = [iTraderCommunicator sharedManager];
		
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
	[self.communicator graphForFeedTicker:self.symbol.feedTicker period:0 width:130 height:130 orientation:@"A"];
	[self setValues];
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
	self.lastChangeLabel.text = [self.symbol.change stringValue];
	self.percentChangeLabel.text = [NSString stringWithFormat:@"%@%%", self.symbol.percentChange];
	self.tickerSymbolLabel.text = self.symbol.tickerSymbol;
	self.lowLabel.text = [self.symbol.low stringValue];
	self.highLabel.text = [self.symbol.high stringValue];
	self.volumeLabel.text = self.symbol.volume;
	self.openLabel.text = [self.symbol.open stringValue];
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

- (void)chartUpdated:(Chart *)chart {
	self.graphImage.image = [chart image];
}

@end
