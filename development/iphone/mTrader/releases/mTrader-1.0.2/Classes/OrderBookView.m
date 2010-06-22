//
//  OrderBookView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010  AS. All rights reserved.
//

#import "OrderBookView.h"
#import "OrderBookController.h"
#import "OrderBookTableCellP.h"
#import "GradientLabel.h"

#import "Symbol.h"
#import "SymbolDynamicData.h"

@implementation OrderBookView
@synthesize symbol = _symbol;
@synthesize viewController = _viewController;
@synthesize orderBookButton, askSizeLabel, askValueLabel, bidSizeLabel, bidValueLabel;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
    if (self = [super initWithFrame:frame]) {
		_symbol = nil;
		
		orderBookController = [[OrderBookController alloc] initWithManagedObjectContext:managedObjectContext];
		[self addSubview:orderBookController.view];
		
		super.padding = 6.0f;
		super.cornerRadius = 10.0f;
		super.strokeWidth = 0.75f;
		
		UIFont *headerFont = [UIFont boldSystemFontOfSize:18.0];
		
		CGFloat leftPadding = floorf(super.padding + super.strokeWidth * 2.0 + kBlur);
		CGFloat maxWidth = self.bounds.size.width - (leftPadding * 2.0f);
		CGFloat maxHeight = self.bounds.size.height - (leftPadding * 2.0f);
		
		CGSize headerFontSize = [@"X" sizeWithFont:headerFont];
		CGFloat labelWidth = floorf(maxWidth / 4.0f);
		CGRect bidSizeLabelFrame = CGRectMake(leftPadding, leftPadding, labelWidth, headerFontSize.height);
		CGRect bidValueLabelFrame = CGRectMake(leftPadding + labelWidth, leftPadding, labelWidth, headerFontSize.height);
		CGRect askValueLabelFrame = CGRectMake(leftPadding + labelWidth * 2.0f, leftPadding, labelWidth, headerFontSize.height);
		CGRect askSizeLabelFrame = CGRectMake(leftPadding + labelWidth * 3.0f, leftPadding, labelWidth, headerFontSize.height);
		
		
		CGRect tableFrame = CGRectMake(leftPadding, leftPadding + headerFontSize.height, maxWidth, maxHeight - headerFontSize.height);
		
		orderBookButton = [[UIButton alloc] initWithFrame:self.bounds];
		[orderBookButton addTarget:self.viewController action:@selector(orderBook:) forControlEvents:UIControlEventTouchUpInside];
		
		askSizeLabel = [[GradientLabel alloc] initWithFrame:askSizeLabelFrame];
		askSizeLabel.textAlignment = UITextAlignmentCenter;
		askSizeLabel.font = headerFont;
		askSizeLabel.text = @"A Size";
		
		askValueLabel = [[GradientLabel alloc] initWithFrame:askValueLabelFrame];
		askValueLabel.textAlignment = UITextAlignmentCenter;
		askValueLabel.font = headerFont;
		askValueLabel.text = @"A Price";
		
		bidSizeLabel = [[GradientLabel alloc] initWithFrame:bidSizeLabelFrame];
		bidSizeLabel.textAlignment = UITextAlignmentCenter;
		bidSizeLabel.font = headerFont;
		bidSizeLabel.text = @"B Size";
		
		bidValueLabel = [[GradientLabel alloc] initWithFrame:bidValueLabelFrame];
		bidValueLabel.textAlignment = UITextAlignmentCenter;
		bidValueLabel.font = headerFont;
		bidValueLabel.text = @"B Price";
		
		orderBookController.view.frame = tableFrame;
		
		[self addSubview:orderBookButton];
		[orderBookButton addSubview:askSizeLabel];
		[orderBookButton addSubview:askValueLabel];
		[orderBookButton addSubview:bidSizeLabel];
		[orderBookButton addSubview:bidValueLabel];
		
    }
    return self;
}

- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	orderBookController.symbol = _symbol;
}


#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[askSizeLabel release];
	[askValueLabel release];
	[bidValueLabel release];
	[bidSizeLabel release];
	[orderBookButton release];
	
	[_symbol release];
	
	[orderBookController release];

    [super dealloc];
}

@end
