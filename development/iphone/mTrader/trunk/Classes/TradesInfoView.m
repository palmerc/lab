//
//  TradesInfoView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 28.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesInfoView.h"

#import "Symbol.h"

#import "TradesController.h"

@implementation TradesInfoView

@synthesize symbol = _symbol;
@synthesize tradesController = _tradesController; 
@synthesize viewController = _viewController;
@synthesize tradesButton = _tradesButton;
@synthesize timeLabel = _timeLabel;
@synthesize buyerSellerLabel = _buyerSellerLabel;
@synthesize priceLabel = _priceLabel;
@synthesize sizeLabel = _sizeLabel;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
    if (self = [super initWithFrame:frame]) {
		_symbol = nil;
		
		_tradesController = [[TradesController alloc] initWithManagedObjectContext:managedObjectContext];
		[self addSubview:_tradesController.view];
		
		_viewController = nil;
		_timeLabel = nil;
		_buyerSellerLabel = nil;
		_priceLabel = nil;
		_sizeLabel = nil;
		_tradesButton = nil;
    }
    return self;
}

#pragma mark -
#pragma mark UIView drawing
- (void)drawRect:(CGRect)rect {
	
  	super.padding = 6.0f;
	super.cornerRadius = 10.0f;
	super.strokeWidth = 0.75f;
	
	UIFont *headerFont = [UIFont boldSystemFontOfSize:18.0];
	
	CGFloat leftPadding = floorf(super.padding + super.strokeWidth * 2.0 + kBlur);
	CGFloat maxWidth = rect.size.width - leftPadding * 2.0f;
	CGFloat maxHeight = rect.size.height - leftPadding * 2.0f;
	
	CGSize headerFontSize = [@"X" sizeWithFont:headerFont];
	CGSize timeFontSize = [@"XX:XX:XX" sizeWithFont:headerFont];
	CGFloat fifthWidth = floorf(maxWidth / 5.0f);
	CGRect timeLabelFrame = CGRectMake(leftPadding, leftPadding, timeFontSize.width, headerFontSize.height);
	CGRect buyerSellerLabelFrame = CGRectMake(leftPadding + fifthWidth, leftPadding, fifthWidth, headerFontSize.height);
	CGRect sizeLabelFrame = CGRectMake(leftPadding + fifthWidth * 2.0f, leftPadding, fifthWidth, headerFontSize.height);
	CGRect priceLabelFrame = CGRectMake(leftPadding + fifthWidth * 3.0f, leftPadding, fifthWidth, headerFontSize.height);
	
	CGRect tableFrame = CGRectMake(leftPadding, leftPadding + headerFontSize.height, maxWidth, maxHeight - headerFontSize.height);
	
	_tradesButton = [[UIButton alloc] initWithFrame:rect];
	[_tradesButton addTarget:self.viewController action:@selector(trades:) forControlEvents:UIControlEventTouchUpInside];
	
	_timeLabel = [[UILabel alloc] initWithFrame:timeLabelFrame];
	_timeLabel.textAlignment = UITextAlignmentCenter;
	_timeLabel.font = headerFont;
	_timeLabel.text = @"Time";
	
	_priceLabel = [[UILabel alloc] initWithFrame:priceLabelFrame];
	_priceLabel.textAlignment = UITextAlignmentRight;
	_priceLabel.font = headerFont;
	_priceLabel.text = @"Price";
	
	_sizeLabel = [[UILabel alloc] initWithFrame:sizeLabelFrame];
	_sizeLabel.textAlignment = UITextAlignmentRight;
	_sizeLabel.font = headerFont;
	_sizeLabel.text = @"Volume";
	
	_buyerSellerLabel = [[UILabel alloc] initWithFrame:buyerSellerLabelFrame];
	_buyerSellerLabel.textAlignment = UITextAlignmentLeft;
	_buyerSellerLabel.font = headerFont;
	_buyerSellerLabel.text = @"B/S";
	
	_tradesController.view.frame = tableFrame;
	
	[self addSubview:_tradesButton];
	[_tradesButton addSubview:_timeLabel];
	[_tradesButton addSubview:_buyerSellerLabel];
	[_tradesButton addSubview:_priceLabel];
	[_tradesButton addSubview:_sizeLabel];
	
	[super drawRect:rect];
}

- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	_tradesController.symbol = _symbol;
}


#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_timeLabel release];
	[_buyerSellerLabel release];
	[_priceLabel release];
	[_sizeLabel release];
	[_tradesButton release];
	
	[_symbol release];
	
	[_viewController release];
	[_tradesController release];
	
    [super dealloc];
}

@end
