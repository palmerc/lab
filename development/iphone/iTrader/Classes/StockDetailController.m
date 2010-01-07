//
//  StockDetailController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "StockDetailController.h"
#import "Symbol.h"

@implementation StockDetailController
@synthesize symbol = _symbol;
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
		self.title = self.symbol.tickerSymbol;
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
		
	self.stockNameLabel.text = self.symbol.name;
	self.stockISINLabel.text = self.symbol.isin;
	self.exchangeLabel.text = self.symbol.feedNumber;
	self.lastChangeLabel.text = [self.symbol.change stringValue];
	self.percentChangeLabel.text = [self.symbol.percentChange stringValue];
	self.tickerSymbolLabel.text = self.symbol.tickerSymbol;
	self.lowLabel.text = [self.symbol.low stringValue];
	self.highLabel.text = [self.symbol.high stringValue];
	self.volumeLabel.text = [self.symbol.volume stringValue];
	self.openLabel.text = [self.symbol.open stringValue];
	self.graphImage.image = [UIImage imageNamed:@"infront.png"];
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

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
