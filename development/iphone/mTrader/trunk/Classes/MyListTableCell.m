//
//  MyListTableCell.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "MyListTableCell.h"

#import "Symbol.h"
#import "SymbolDynamicData.h"


#pragma mark -
#pragma mark SubviewFrames category

@interface MyListTableCell (SubviewFrames)
- (CGRect)_tickerLabelFrame;
- (CGRect)_descriptionLabelFrame;
- (CGRect)_centerButtonFrame;
- (CGRect)_rightButtonFrame;
- (CGRect)_timeLabelFrame;
@end



#pragma mark -
#pragma mark MyListTableCell implementation
@implementation MyListTableCell
@synthesize symbolDynamicData, tickerLabel, descriptionLabel, centerButton, rightButton, timeLabel, centerButtonOption, rightButtonOption;


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
		
		centerButton = [[UIButton alloc] initWithFrame:CGRectZero];
		[centerButton.titleLabel setFont:[UIFont systemFontOfSize:17.0]];
		[centerButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
		[self.contentView addSubview:centerButton];
		
		rightButton = [[UIButton alloc] initWithFrame:CGRectZero];
		[rightButton.titleLabel setFont:[UIFont systemFontOfSize:17.0]];
		[rightButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
		[self.contentView addSubview:rightButton];
		
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
    [centerButton setFrame:[self _centerButtonFrame]];
    [rightButton setFrame:[self _rightButtonFrame]];
	[timeLabel setFrame:[self _timeLabelFrame]];
    if (self.editing) {
        centerButton.alpha = 0.0;
		rightButton.alpha = 0.0;
		timeLabel.alpha = 0.0;
    } else {
        centerButton.alpha = 1.0;
		rightButton.alpha = 1.0;
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
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 4.0, self.contentView.bounds.size.width - EDITING_INSET - TEXT_LEFT_MARGIN, 16.0);
    }
	else {
        return CGRectMake(TEXT_LEFT_MARGIN, 2.0, tickerLabelSize.width, tickerLabelSize.height);
    }
}

- (CGRect)_descriptionLabelFrame {
	
	if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 4.0, self.contentView.bounds.size.width - EDITING_INSET - TEXT_LEFT_MARGIN, 16.0);
    }
	else {
        return CGRectMake(TEXT_LEFT_MARGIN, 24.0, DESCRIPTION_WIDTH, 16.0);
    }
}

- (CGRect)_centerButtonFrame {
    if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 4.0, self.contentView.bounds.size.width - EDITING_INSET - TEXT_LEFT_MARGIN, 16.0);
    }
	else {
        return CGRectMake(TEXT_LEFT_MARGIN + tickerLabelSize.width, 4.0, BUTTON_WIDTH, 16.0);
    }
}

- (CGRect)_rightButtonFrame {
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
	
	tickerLabel.text = symbolDynamicData.symbol.tickerSymbol;
	descriptionLabel.text = symbolDynamicData.symbol.companyName;

	NSString *centerString = nil;
	switch (centerButtonOption) {
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
	[centerButton setTitle:centerString forState:UIControlStateNormal];
	
	NSString *rightString = nil;
	switch (rightButtonOption) {
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
	[rightButton setTitle:rightString forState:UIControlStateNormal];

	timeLabel.text = [dateFormatter stringFromDate:symbolDynamicData.lastTradeTime];
}



#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[symbolDynamicData release];
	[tickerLabel release];
	[descriptionLabel release];
	[centerButton release];
	[rightButton release];
	[timeLabel release];
	
    [super dealloc];
}


@end
