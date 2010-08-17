//
//  OrderBookView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010  AS. All rights reserved.
//

#define DEBUG 0

#import "OrderBookView.h"
#import "OrderBookController.h"
#import "OrderBookTableCellP.h"

#import "Symbol.h"
#import "SymbolDynamicData.h"

@implementation OrderBookView
@synthesize symbol = _symbol;
@synthesize viewController = _viewController;
@synthesize orderBookButton = _orderBookButton;
@synthesize askSizeLabel = _askSizeLabel;
@synthesize askValueLabel = _askValueLabel;
@synthesize bidSizeLabel = _bidSizeLabel;
@synthesize bidValueLabel =  _bidValueLabel;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
    if (self = [super initWithFrame:frame]) {
//		_symbol = nil;
//		
//		_orderBookController = [[OrderBookController alloc] initWithManagedObjectContext:managedObjectContext];
//		[self addSubview:_orderBookController.view];
//		
//		super.padding = 6.0f;
//		super.cornerRadius = 10.0f;
//		super.strokeWidth = 0.75f;
//		
//		UIFont *headerFont = [UIFont boldSystemFontOfSize:18.0];
//		
//		CGFloat leftPadding = floorf(super.padding + super.strokeWidth * 2.0 + kBlur);
//		CGFloat maxWidth = self.bounds.size.width - (leftPadding * 2.0f);
//		CGFloat maxHeight = self.bounds.size.height - (leftPadding * 2.0f);
//		
//		CGSize headerFontSize = [@"X" sizeWithFont:headerFont];
//		CGFloat labelWidth = floorf(maxWidth / 4.0f);
//		CGRect bidSizeLabelFrame = CGRectMake(leftPadding, leftPadding, labelWidth, headerFontSize.height);
//		CGRect bidValueLabelFrame = CGRectMake(leftPadding + labelWidth, leftPadding, labelWidth, headerFontSize.height);
//		CGRect askValueLabelFrame = CGRectMake(leftPadding + labelWidth * 2.0f, leftPadding, labelWidth, headerFontSize.height);
//		CGRect askSizeLabelFrame = CGRectMake(leftPadding + labelWidth * 3.0f, leftPadding, labelWidth, headerFontSize.height);
//				
//		CGRect tableFrame = CGRectMake(leftPadding, leftPadding + headerFontSize.height, maxWidth, maxHeight - headerFontSize.height);
//		
//		_orderBookButton = [[UIButton alloc] initWithFrame:self.bounds];
//		[_orderBookButton addTarget:self.viewController action:@selector(orderBook:) forControlEvents:UIControlEventTouchUpInside];
//		
//		_askSizeLabel = [[UILabel alloc] initWithFrame:askSizeLabelFrame];
//		_askSizeLabel.textAlignment = UITextAlignmentCenter;
//		_askSizeLabel.font = headerFont;
//		_askSizeLabel.text = NSLocalizedString(@"askSize", @"A Size");
//		
//		_askValueLabel = [[UILabel alloc] initWithFrame:askValueLabelFrame];
//		_askValueLabel.textAlignment = UITextAlignmentCenter;
//		_askValueLabel.font = headerFont;
//		_askValueLabel.text = NSLocalizedString(@"askPrice", @"A Price");
//		
//		_bidSizeLabel = [[UILabel alloc] initWithFrame:bidSizeLabelFrame];
//		_bidSizeLabel.textAlignment = UITextAlignmentCenter;
//		_bidSizeLabel.font = headerFont;
//		_bidSizeLabel.text = NSLocalizedString(@"bidSize", @"B Size");
//		
//		_bidValueLabel = [[UILabel alloc] initWithFrame:bidValueLabelFrame];
//		_bidValueLabel.textAlignment = UITextAlignmentCenter;
//		_bidValueLabel.font = headerFont;
//		_bidValueLabel.text = NSLocalizedString(@"bidPrice", @"B Price");
//		
//		_orderBookController.view.frame = tableFrame;
//		
//		[self addSubview:_orderBookButton];
//		[_orderBookButton addSubview:_askSizeLabel];
//		[_orderBookButton addSubview:_askValueLabel];
//		[_orderBookButton addSubview:_bidSizeLabel];
//		[_orderBookButton addSubview:_bidValueLabel];
	}
	
    return self;
}

- (void)setSymbol:(Symbol *)symbol {
	if (symbol == _symbol) {
		return;
	}
	
	[_symbol release];
	_symbol = [symbol retain];
	_orderBookController.symbol = _symbol;
	
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_askSizeLabel release];
	[_askValueLabel release];
	[_bidValueLabel release];
	[_bidSizeLabel release];
	[_orderBookButton release];
	
	[_symbol release];
	
	[_viewController release];
	[_orderBookController release];

    [super dealloc];
}

@end
