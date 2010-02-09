//
//  ChainsTableCell.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "ChainsTableCell.h"

#import "Symbol.h"
#import "SymbolDynamicData.h"


#pragma mark -
#pragma mark SubviewFrames category

@interface ChainsTableCell (SubviewFrames)
- (CGRect)_tickerLabelFrame;
- (CGRect)_descriptionLabelFrame;
- (CGRect)_centerLabelFrame;
- (CGRect)_rightLabelFrame;
- (CGRect)_timeLabelFrame;
@end



#pragma mark -
#pragma mark ChainsTableCell implementation
@implementation ChainsTableCell
@synthesize symbolDynamicData, tickerLabel, descriptionLabel, centerLabel, rightLabel, timeLabel, centerOption, rightOption;

#pragma mark -
#pragma mark Initialization
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {		
		UIFont *tickerFont = [UIFont boldSystemFontOfSize:17.0];
		tickerLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[tickerLabel setBackgroundColor:[UIColor whiteColor]];
		[tickerLabel setFont:tickerFont];
		[tickerLabel setTextColor:[UIColor blackColor]];
		[tickerLabel setHighlightedTextColor:[UIColor whiteColor]];
		[self.contentView addSubview:tickerLabel];
		
		NSString *tickerSample = @"XXXXXXXXX";
		tickerLabelSize = [tickerSample sizeWithFont:tickerFont];
		
		UIFont *descriptionFont = [UIFont systemFontOfSize:12.0];
		descriptionLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		descriptionLabel.textAlignment = UITextAlignmentLeft;
		[descriptionLabel setFont:descriptionFont];
		[descriptionLabel setTextColor:[UIColor lightGrayColor]];
		[descriptionLabel setHighlightedTextColor:[UIColor whiteColor]];
		[self.contentView addSubview:descriptionLabel];
		
		UIFont *centerLabelFont = [UIFont systemFontOfSize:17.0];
		centerLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[centerLabel setFont:centerLabelFont];
		[centerLabel setTextAlignment:UITextAlignmentRight];
		[centerLabel setTextColor:[UIColor blackColor]];
		[centerLabel setHighlightedTextColor:[UIColor whiteColor]];
		[self.contentView addSubview:centerLabel];		
		
		UIFont *rightLabelFont = [UIFont systemFontOfSize:17.0];
		rightLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[rightLabel setFont:rightLabelFont];
		[rightLabel setTextAlignment:UITextAlignmentRight];
		[rightLabel setTextColor:[UIColor blackColor]];
		[rightLabel setHighlightedTextColor:[UIColor whiteColor]];
		[self.contentView addSubview:rightLabel];
		
		UIFont *timeFont = [UIFont systemFontOfSize:12.0];
		timeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		timeLabel.textAlignment = UITextAlignmentRight;
		[timeLabel setFont:timeFont];
		[timeLabel setTextColor:[UIColor lightGrayColor]];
		[timeLabel setHighlightedTextColor:[UIColor whiteColor]];
		[self.contentView addSubview:timeLabel];
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
	
    [tickerLabel setFrame:[self _tickerLabelFrame]];
	[descriptionLabel setFrame:[self _descriptionLabelFrame]];
	[centerLabel setFrame:[self _centerLabelFrame]];
	[rightLabel setFrame:[self _rightLabelFrame]];
	[timeLabel setFrame:[self _timeLabelFrame]];
    if (self.editing) {
        centerLabel.alpha = 0.0;
		rightLabel.alpha = 0.0;
		timeLabel.alpha = 0.0;
    } else {
        centerLabel.alpha = 1.0;
		rightLabel.alpha = 1.0;
		timeLabel.alpha = 1.0;
    }
}


#define EDITING_INSET       10.0
#define TEXT_LEFT_MARGIN    8.0
#define TEXT_RIGHT_MARGIN   5.0
#define BUTTON_WIDTH        100.0
#define DESCRIPTION_WIDTH   200.0

/*
 Return the frame of the various subviews -- these are dependent on the editing state of the cell.
 */
- (CGRect)_tickerLabelFrame {

	if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 2.0, self.contentView.bounds.size.width - EDITING_INSET - TEXT_LEFT_MARGIN, 16.0);
    }
	else {
        return CGRectMake(TEXT_LEFT_MARGIN, 2.0, tickerLabelSize.width, tickerLabelSize.height);
    }
}

- (CGRect)_descriptionLabelFrame {
	
	if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 24.0, self.contentView.bounds.size.width - EDITING_INSET - TEXT_LEFT_MARGIN, 16.0);
    }
	else {
        return CGRectMake(TEXT_LEFT_MARGIN, 24.0, DESCRIPTION_WIDTH, 16.0);
    }
}

- (CGRect)_centerLabelFrame {
    if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 4.0, self.contentView.bounds.size.width - EDITING_INSET - TEXT_LEFT_MARGIN, 16.0);
    }
	else {
        return CGRectMake(TEXT_LEFT_MARGIN + tickerLabelSize.width, 4.0, BUTTON_WIDTH, 16.0);
    }
}

- (CGRect)_rightLabelFrame {
    if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 4.0, self.contentView.bounds.size.width - EDITING_INSET - TEXT_LEFT_MARGIN, 16.0);
    }
	else {
        return CGRectMake(TEXT_LEFT_MARGIN + tickerLabelSize.width + BUTTON_WIDTH, 4.0, BUTTON_WIDTH, 16.0);
    }
}

- (CGRect)_timeLabelFrame {
    if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 4.0, self.contentView.bounds.size.width - EDITING_INSET - TEXT_LEFT_MARGIN, 16.0);
    }
	else {
        return CGRectMake(TEXT_LEFT_MARGIN + tickerLabelSize.width + BUTTON_WIDTH, 24.0, BUTTON_WIDTH, 16.0);
    }
}


- (void)setSymbolDynamicData:(SymbolDynamicData *)newSymbolDynamicData {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateStyle:NSDateFormatterShortStyle];
		[dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
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
	
	if (newSymbolDynamicData != symbolDynamicData) {
		[symbolDynamicData release];
		symbolDynamicData = [newSymbolDynamicData retain];
	}
	
	self.tickerLabel.text = symbolDynamicData.symbol.tickerSymbol;
	self.descriptionLabel.text = symbolDynamicData.symbol.companyName;

	NSString *centerString = nil;
	switch (centerOption) {
		case LAST_TRADE:
			centerString = [doubleFormatter stringFromNumber:symbolDynamicData.lastTrade];
			break;
		case BID_PRICE:
			centerString = [doubleFormatter stringFromNumber:symbolDynamicData.bidPrice];
			break;
		case ASK_PRICE:
			centerString = [doubleFormatter stringFromNumber:symbolDynamicData.askPrice];
			break;
		default:
			break;
	}
	self.centerLabel.text = centerString;

	NSString *rightString = nil;
	switch (rightOption) {
		case LAST_TRADE_PERCENT_CHANGE:
			rightString = [percentFormatter stringFromNumber:symbolDynamicData.lastTradePercentChange];
			break;
		case LAST_TRADE_CHANGE:
			rightString = [doubleFormatter stringFromNumber:symbolDynamicData.lastTradeChange];
			break;
		case LAST_TRADE_TOO:
			rightString = [doubleFormatter stringFromNumber:symbolDynamicData.lastTrade];
			break;
		default:
			break;
	}
	self.rightLabel.text = rightString;
	
	timeLabel.text = [dateFormatter stringFromDate:symbolDynamicData.lastTradeTime];
}



#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[symbolDynamicData release];
	[tickerLabel release];
	[descriptionLabel release];
	[centerLabel release];
	[rightLabel release];
	[timeLabel release];
	
    [super dealloc];
}


@end
