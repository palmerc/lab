//
//  OrderBookCellP.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "OrderBookTableCellP.h"
#import "Symbol.h"

#pragma mark -
#pragma mark SubviewFrames category

@interface OrderBookTableCellP (SubviewFrames)
- (CGRect)_bidSizeLabelFrame;
- (CGRect)_bidValueLabelFrame;
- (CGRect)_askSizeLabelFrame;
- (CGRect)_askValueLabelFrame;
@end


#pragma mark -
#pragma mark ChainsTableCell implementation
@implementation OrderBookTableCellP
@synthesize symbol, bidSizeLabel, bidValueLabel, askSizeLabel, askValueLabel;

#pragma mark -
#pragma mark Initialization
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {		
	}
    return self;
}

#pragma mark -
#pragma mark Laying out subviews

/*
 To save space, the prep time label disappears during editing.
 */
- (void)layoutSubviews {
    [super layoutSubviews];
	
    [bidSizeLabel setFrame:[self _bidSizeLabelFrame]];
	[bidValueLabel setFrame:[self _bidValueLabelFrame]];
	[askSizeLabel setFrame:[self _askSizeLabelFrame]];
	[askValueLabel setFrame:[self _askValueLabelFrame]];

}


#define EDITING_INSET       10.0
#define TEXT_LEFT_MARGIN    8.0
#define TEXT_RIGHT_MARGIN   8.0
#define BUTTON_WIDTH        100.0
#define TIME_WIDTH          102.0
#define DESCRIPTION_WIDTH   200.0

/*
 Return the frame of the various subviews -- these are dependent on the editing state of the cell.
 */
- (CGRect)_bidSizeLabelFrame {
	return CGRectMake(TEXT_LEFT_MARGIN, 2.0, 50.0, 16.0);
}

- (CGRect)_bidValueLabelFrame {
	return CGRectMake(TEXT_LEFT_MARGIN, 24.0, DESCRIPTION_WIDTH, 16.0);
}

- (CGRect)_askSizeLabelFrame {
	return CGRectMake(TEXT_LEFT_MARGIN, 4.0, BUTTON_WIDTH, 16.0);
}

- (CGRect)_askValueLabelFrame {
	return CGRectMake(TEXT_LEFT_MARGIN + BUTTON_WIDTH, 4.0, BUTTON_WIDTH, 16.0);
}

- (void)setOrderBookData:(Symbol *)newSymbol {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateStyle:NSDateFormatterShortStyle];
	}
	
	static NSDateFormatter *timeFormatter = nil;
	if (timeFormatter == nil) {
		timeFormatter = [[NSDateFormatter alloc] init];
		[timeFormatter setDateFormat:@"HH:mm:ss"];
	}
	
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
		[doubleFormatter setUsesSignificantDigits:YES];
	}
	
	static NSNumberFormatter *percentFormatter = nil;
	if (percentFormatter == nil) {
		percentFormatter = [[NSNumberFormatter alloc] init];
		[percentFormatter setNumberStyle:NSNumberFormatterPercentStyle];
		[percentFormatter setUsesSignificantDigits:YES];
	}
	
	if (newSymbol != symbol) {
		[symbol release];
		symbol = [newSymbol retain];
	}
	
	self.bidSizeLabel.text = @"";
	self.bidValueLabel.text = @"";
	self.askSizeLabel.text = @"";
	self.askValueLabel.text = @"";
	
	//NSDate *tradeTime = @"";
	//NSString *timeString = [NSString stringWithFormat:@"%@ %@", [dateFormatter stringFromDate:tradeTime], [timeFormatter stringFromDate:tradeTime]];
	//self.timeLabel.text = timeString;
}



#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[bidSizeLabel release];
	[bidValueLabel release];
	[askSizeLabel release];
	[askValueLabel release];
	
    [super dealloc];
}

@end
