//
//  ChainsTableCell.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "ChainsTableCell.h"

#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"


#pragma mark -
#pragma mark SubviewFrames category

@interface ChainsTableCell (SubviewFrames)
- (CGRect)_tickerLabelFrame;
- (CGRect)_descriptionLabelFrame;
- (CGRect)_centerLabelFrame;
- (CGRect)_rightLabelFrame;
- (CGRect)_currencyLabelFrame;
- (CGRect)_timeLabelFrame;
@end



#pragma mark -
#pragma mark ChainsTableCell implementation
@implementation ChainsTableCell
@synthesize symbolDynamicData = _symbolDynamicData; 
@synthesize tickerLabel = _tickerLabel;
@synthesize descriptionLabel = _descriptionLabel;
@synthesize centerLabel = _centerLabel;
@synthesize rightLabel = _rightLabel;
@synthesize currencyLabel = _currencyLabel;
@synthesize timeLabel = _timeLabel;
@synthesize centerOption, rightOption;

#pragma mark -
#pragma mark Initialization
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {	
		UIFont *tickerFont = [UIFont boldSystemFontOfSize:17.0];
		_tickerLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_tickerLabel setBackgroundColor:[UIColor clearColor]];
		[_tickerLabel setFont:tickerFont];
		[_tickerLabel setTextColor:[UIColor blackColor]];
		//[tickerLabel setHighlightedTextColor:[UIColor blackColor]];
		[self.contentView addSubview:_tickerLabel];
		
		NSString *tickerSample = @"XXXXXXXXXXXX";
		tickerLabelSize = [tickerSample sizeWithFont:tickerFont];
		
		UIFont *descriptionFont = [UIFont systemFontOfSize:12.0];
		_descriptionLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_descriptionLabel setBackgroundColor:[UIColor clearColor]];
		_descriptionLabel.textAlignment = UITextAlignmentLeft;
		[_descriptionLabel setFont:descriptionFont];
		[_descriptionLabel setTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:_descriptionLabel];
		
		UIFont *centerLabelFont = [UIFont systemFontOfSize:17.0];
		_centerLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_centerLabel setBackgroundColor:[UIColor clearColor]];
		[_centerLabel setFont:centerLabelFont];
		[_centerLabel setTextAlignment:UITextAlignmentRight];
		[_centerLabel setTextColor:[UIColor blackColor]];
		[_centerLabel setAdjustsFontSizeToFitWidth:YES];
		[self.contentView addSubview:_centerLabel];		
		
		UIFont *rightLabelFont = [UIFont systemFontOfSize:17.0];
		_rightLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_rightLabel setBackgroundColor:[UIColor clearColor]];
		[_rightLabel setFont:rightLabelFont];
		[_rightLabel setTextAlignment:UITextAlignmentRight];
		[_rightLabel setTextColor:[UIColor blackColor]];
		[_rightLabel setAdjustsFontSizeToFitWidth:YES];
		[self.contentView addSubview:_rightLabel];
		
		UIFont *currencyFont = [UIFont systemFontOfSize:12.0];
		_currencyLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_currencyLabel setBackgroundColor:[UIColor clearColor]];
		_currencyLabel.textAlignment = UITextAlignmentRight;
		[_currencyLabel setFont:currencyFont];
		[_currencyLabel setTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:_currencyLabel];
		
		UIFont *timeFont = [UIFont systemFontOfSize:12.0];
		_timeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_timeLabel setBackgroundColor:[UIColor clearColor]];
		_timeLabel.textAlignment = UITextAlignmentRight;
		[_timeLabel setFont:timeFont];
		[_timeLabel setTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:_timeLabel];
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
	
    [_tickerLabel setFrame:[self _tickerLabelFrame]];
	[_descriptionLabel setFrame:[self _descriptionLabelFrame]];
	[_centerLabel setFrame:[self _centerLabelFrame]];
	[_rightLabel setFrame:[self _rightLabelFrame]];
	[_currencyLabel setFrame:[self _currencyLabelFrame]];
	[_timeLabel setFrame:[self _timeLabelFrame]];
    if (self.editing) {
        _centerLabel.alpha = 0.0;
		_rightLabel.alpha = 0.0;
		_currencyLabel.alpha = 0.0;
		_timeLabel.alpha = 0.0;
    } else {
        _centerLabel.alpha = 1.0;
		_rightLabel.alpha = 1.0;
		_currencyLabel.alpha = 1.0;
		_timeLabel.alpha = 1.0;
    }
}


#define EDITING_INSET       10.0
#define TEXT_LEFT_MARGIN    8.0
#define TEXT_RIGHT_MARGIN   8.0
#define BUTTON_WIDTH        85.0
#define TIME_WIDTH          102.0
#define DESCRIPTION_WIDTH   200.0

/*
 Return the frame of the various subviews -- these are dependent on the editing state of the cell.
 */
- (CGRect)_tickerLabelFrame {

	if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 2.0, self.frame.size.width - EDITING_INSET - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN, 16.0);
    } else {
        return CGRectMake(TEXT_LEFT_MARGIN, 2.0, self.frame.size.width - 2 * BUTTON_WIDTH - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN, 16.0);
    }
}

- (CGRect)_descriptionLabelFrame {
	
	if (self.editing) {
        return CGRectMake(EDITING_INSET + TEXT_LEFT_MARGIN, 24.0, self.frame.size.width - EDITING_INSET - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN, 16.0);
    } else {
        return CGRectMake(TEXT_LEFT_MARGIN, 24.0, self.frame.size.width - 2 * BUTTON_WIDTH - TEXT_LEFT_MARGIN - TEXT_RIGHT_MARGIN, 16.0);
    }
}

- (CGRect)_centerLabelFrame {
    return CGRectMake(self.frame.size.width - BUTTON_WIDTH * 2 - TEXT_RIGHT_MARGIN, 4.0, BUTTON_WIDTH, 16.0);
}

- (CGRect)_rightLabelFrame {
    return CGRectMake(self.frame.size.width - BUTTON_WIDTH - TEXT_RIGHT_MARGIN, 4.0, BUTTON_WIDTH, 16.0);
}

- (CGRect)_currencyLabelFrame {
    return CGRectMake(self.frame.size.width - 2 * BUTTON_WIDTH - TEXT_RIGHT_MARGIN, 24.0, BUTTON_WIDTH, 16.0);
}

- (CGRect)_timeLabelFrame {
    return CGRectMake(self.frame.size.width - BUTTON_WIDTH - TEXT_RIGHT_MARGIN, 24.0, BUTTON_WIDTH, 16.0);
}


- (void)setSymbolDynamicData:(SymbolDynamicData *)newSymbolDynamicData {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
	}
	
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	}
	
	static NSNumberFormatter *percentFormatter = nil;
	if (percentFormatter == nil) {
		percentFormatter = [[NSNumberFormatter alloc] init];
		[percentFormatter setNumberStyle:NSNumberFormatterPercentStyle];
		[percentFormatter setUsesSignificantDigits:YES];
	}
	
	
	if (newSymbolDynamicData != _symbolDynamicData) {
		[_symbolDynamicData release];
		// Although I would expect self.symbolDynamicData to work in this case it does not.
		_symbolDynamicData = [newSymbolDynamicData retain];
	}
	
	self.tickerLabel.text = [NSString stringWithFormat:@"%@ (%@)", self.symbolDynamicData.symbol.tickerSymbol, self.symbolDynamicData.symbol.feed.mCode];
	self.descriptionLabel.text = self.symbolDynamicData.symbol.companyName;

	// Red Font for Down, Blue Font for Up, Black for No Change
	UIColor *textColor = nil;
	
	NSComparisonResult comparison = [self.symbolDynamicData.change compare:[NSNumber numberWithDouble:0.0]];
	switch (comparison) {
		case NSOrderedAscending:
			textColor = [UIColor redColor];
			break;
		case NSOrderedDescending:
			textColor = [UIColor blueColor];
			break;
		case NSOrderedSame:
			textColor = [UIColor blackColor];
			break;
		default:
			textColor = [UIColor blackColor];
			break;
	}

	NSUInteger decimals = [self.symbolDynamicData.symbol.feed.decimals integerValue];
	[doubleFormatter setMinimumFractionDigits:decimals];
	[doubleFormatter setMaximumFractionDigits:decimals];
	
	NSString *centerString = @"-";
	switch (centerOption) {
		case CLAST:
			if (self.symbolDynamicData.lastTrade != nil) {
				centerString = [doubleFormatter stringFromNumber:self.symbolDynamicData.lastTrade];
			}			
			break;
		case CBID:
			if (self.symbolDynamicData.bidPrice != nil) {
				centerString = [doubleFormatter stringFromNumber:self.symbolDynamicData.bidPrice];
			}
			break;
		case CASK:
			if (self.symbolDynamicData.askSize != nil) {
				centerString = [doubleFormatter stringFromNumber:self.symbolDynamicData.askPrice];
			}
			break;
		default:
			break;
	}
	self.centerLabel.text = centerString;
	self.centerLabel.textColor = textColor;

	NSString *rightString = @"-";
	switch (rightOption) {
		case RCHANGE_PERCENT:
			if (self.symbolDynamicData.changePercent != nil) {
				rightString = [percentFormatter stringFromNumber:self.symbolDynamicData.changePercent];
			}
			break;
		case RCHANGE:
			if (self.symbolDynamicData.change != nil) {
				rightString = [doubleFormatter stringFromNumber:self.symbolDynamicData.change];
			}
			break;
		case RLAST:
			if (self.symbolDynamicData.lastTrade != nil) {
				rightString = [doubleFormatter stringFromNumber:self.symbolDynamicData.lastTrade];
			}
			break;
		default:
			break;
	}
	self.rightLabel.text = rightString;
	self.rightLabel.textColor = textColor;
	
	self.currencyLabel.text = self.symbolDynamicData.symbol.currency;
	NSString *timeString = [dateFormatter stringFromDate:self.symbolDynamicData.lastTradeTime];
	self.timeLabel.text = timeString;
}



#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_symbolDynamicData release];
	[_tickerLabel release];
	[_descriptionLabel release];
	[_centerLabel release];
	[_rightLabel release];
	[_currencyLabel release];
	[_timeLabel release];
	
    [super dealloc];
}


@end
