//
//  MyListTableCellPad.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "MyListTableCellPad.h"

#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"


#pragma mark -
#pragma mark SubviewFrames category

@interface MyListTableCellPad (SubviewFrames)
- (CGRect)_tickerLabelFrame;
- (CGRect)_descriptionLabelFrame;

- (CGRect)_bidLabelFrame;
- (CGRect)_askLabelFrame;
- (CGRect)_lastLabelFrame;
- (CGRect)_lastChangeLabelFrame;
- (CGRect)_lastPercentLabelFrame;

- (CGRect)_currencyLabelFrame;
- (CGRect)_timeLabelFrame;
@end

#pragma mark -
#pragma mark MyListTableCellPad implementation
@implementation MyListTableCellPad
@synthesize symbolDynamicData = _symbolDynamicData; 
@synthesize tickerLabel = _tickerLabel;
@synthesize descriptionLabel = _descriptionLabel;

@synthesize bidLabel = _bidLabel;
@synthesize askLabel = _askLabel;
@synthesize lastLabel = _lastLabel;
@synthesize lastChangeLabel = _lastChangeLabel;
@synthesize lastPercentLabel = _lastPercentLabel;

@synthesize currencyLabel = _currencyLabel;
@synthesize timeLabel = _timeLabel;

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
		
		UIFont *descriptionFont = [UIFont systemFontOfSize:12.0];
		_descriptionLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_descriptionLabel setBackgroundColor:[UIColor clearColor]];
		_descriptionLabel.textAlignment = UITextAlignmentLeft;
		[_descriptionLabel setFont:descriptionFont];
		[_descriptionLabel setTextColor:[UIColor darkGrayColor]];
		//[descriptionLabel setHighlightedTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:_descriptionLabel];
		
		UIFont *bidLabelFont = [UIFont systemFontOfSize:17.0];
		_bidLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_bidLabel setBackgroundColor:[UIColor clearColor]];
		[_bidLabel setFont:bidLabelFont];
		[_bidLabel setTextAlignment:UITextAlignmentRight];
		[_bidLabel setTextColor:[UIColor blackColor]];
		//[bidLabel setHighlightedTextColor:[UIColor blackColor]];
		[_bidLabel setAdjustsFontSizeToFitWidth:YES];
		[self.contentView addSubview:_bidLabel];
		
		UIFont *askLabelFont = [UIFont systemFontOfSize:17.0];
		_askLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_askLabel setBackgroundColor:[UIColor clearColor]];
		[_askLabel setFont:askLabelFont];
		[_askLabel setTextAlignment:UITextAlignmentRight];
		[_askLabel setTextColor:[UIColor blackColor]];
		//[askLabel setHighlightedTextColor:[UIColor blackColor]];
		[_askLabel setAdjustsFontSizeToFitWidth:YES];
		[self.contentView addSubview:_askLabel];
		
		UIFont *lastLabelFont = [UIFont systemFontOfSize:17.0];
		_lastLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_lastLabel setBackgroundColor:[UIColor clearColor]];
		[_lastLabel setFont:lastLabelFont];
		[_lastLabel setTextAlignment:UITextAlignmentRight];
		[_lastLabel setTextColor:[UIColor blackColor]];
		//[lastLabel setHighlightedTextColor:[UIColor blackColor]];
		[_lastLabel setAdjustsFontSizeToFitWidth:YES];
		[self.contentView addSubview:_lastLabel];		
		
		UIFont *lastChangeLabelFont = [UIFont systemFontOfSize:17.0];
		_lastChangeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_lastChangeLabel setBackgroundColor:[UIColor clearColor]];
		[_lastChangeLabel setFont:lastChangeLabelFont];
		[_lastChangeLabel setTextAlignment:UITextAlignmentRight];
		[_lastChangeLabel setTextColor:[UIColor blackColor]];
		//[lastChangeLabel setHighlightedTextColor:[UIColor blackColor]];
		[_lastChangeLabel setAdjustsFontSizeToFitWidth:YES];
		[self.contentView addSubview:_lastChangeLabel];
		
		UIFont *lastPercentLabelFont = [UIFont systemFontOfSize:17.0];
		_lastPercentLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_lastPercentLabel setBackgroundColor:[UIColor clearColor]];
		[_lastPercentLabel setFont:lastPercentLabelFont];
		[_lastPercentLabel setTextAlignment:UITextAlignmentRight];
		[_lastPercentLabel setTextColor:[UIColor blackColor]];
		//[lastPercentLabel setHighlightedTextColor:[UIColor blackColor]];
		[_lastPercentLabel setAdjustsFontSizeToFitWidth:YES];
		[self.contentView addSubview:_lastPercentLabel];
		
		UIFont *currencyFont = [UIFont systemFontOfSize:12.0];
		_currencyLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_currencyLabel setBackgroundColor:[UIColor clearColor]];
		_currencyLabel.textAlignment = UITextAlignmentRight;
		[_currencyLabel setFont:currencyFont];
		[_currencyLabel setTextColor:[UIColor darkGrayColor]];
		//[timeLabel setHighlightedTextColor:[UIColor darkGrayColor]];
		[self.contentView addSubview:_currencyLabel];
		
		UIFont *timeFont = [UIFont systemFontOfSize:12.0];
		_timeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		[_timeLabel setBackgroundColor:[UIColor clearColor]];
		_timeLabel.textAlignment = UITextAlignmentRight;
		[_timeLabel setFont:timeFont];
		[_timeLabel setTextColor:[UIColor darkGrayColor]];
		//[timeLabel setHighlightedTextColor:[UIColor darkGrayColor]];
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
	
	[_bidLabel setFrame:[self _bidLabelFrame]];
	[_askLabel setFrame:[self _askLabelFrame]];
	[_lastLabel setFrame:[self _lastLabelFrame]];
	[_lastChangeLabel setFrame:[self _lastChangeLabelFrame]];
	[_lastPercentLabel setFrame:[self _lastPercentLabelFrame]];
	
	[_currencyLabel setFrame:[self _currencyLabelFrame]];
	[_timeLabel setFrame:[self _timeLabelFrame]];
    if (self.editing) {
		_bidLabel.alpha = 0.0;
		_askLabel.alpha = 0.0;
        _lastLabel.alpha = 0.0;
		_lastChangeLabel.alpha = 0.0;
		_lastPercentLabel.alpha = 0.0;
		_currencyLabel.alpha = 0.0;
		_timeLabel.alpha = 0.0;
    } else {
		_bidLabel.alpha = 1.0;
		_askLabel.alpha = 1.0;
        _lastLabel.alpha = 1.0;
		_lastChangeLabel.alpha = 1.0;
		_lastPercentLabel.alpha = 1.0;
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

- (CGRect)_bidLabelFrame {
    return CGRectMake(self.frame.size.width - BUTTON_WIDTH * 5 - TEXT_RIGHT_MARGIN, 4.0, BUTTON_WIDTH, 16.0);
}

- (CGRect)_askLabelFrame {
	return CGRectMake(self.frame.size.width - BUTTON_WIDTH * 4 - TEXT_RIGHT_MARGIN, 4.0, BUTTON_WIDTH, 16.0);
}

- (CGRect)_lastLabelFrame {
    return CGRectMake(self.frame.size.width - BUTTON_WIDTH * 3 - TEXT_RIGHT_MARGIN, 4.0, BUTTON_WIDTH, 16.0);
}

- (CGRect)_lastChangeLabelFrame {
    return CGRectMake(self.frame.size.width - BUTTON_WIDTH * 2 - TEXT_RIGHT_MARGIN, 4.0, BUTTON_WIDTH, 16.0);
}

- (CGRect)_lastPercentLabelFrame {
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

	if (self.symbolDynamicData.bidPrice != nil) {
		self.bidLabel.text = [doubleFormatter stringFromNumber:self.symbolDynamicData.bidPrice];
	} else {
		self.bidLabel.text = @"-";
	}
	self.bidLabel.textColor = textColor;
	
	if (self.symbolDynamicData.askPrice != nil) {
		self.askLabel.text = [doubleFormatter stringFromNumber:self.symbolDynamicData.askPrice];
	} else {
		self.askLabel.text = @"-";
	}
	self.askLabel.textColor = textColor;
	
	if (self.symbolDynamicData.lastTrade != nil) {
		self.lastLabel.text = [doubleFormatter stringFromNumber:self.symbolDynamicData.lastTrade];
	} else {
		self.lastLabel.text = @"-";
	}
	self.lastLabel.textColor = textColor;
	
	if (self.symbolDynamicData.changePercent != nil) {
		self.lastPercentLabel.text = [percentFormatter stringFromNumber:self.symbolDynamicData.changePercent];
	} else {
		self.lastPercentLabel.text = @"-";
	}
	self.lastPercentLabel.textColor = textColor;
		
	if (self.symbolDynamicData.change != nil) {
		self.lastChangeLabel.text = [doubleFormatter stringFromNumber:self.symbolDynamicData.change];
	} else {
		self.lastChangeLabel.text = @"-";
	}
	self.lastChangeLabel.textColor = textColor;
		
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
	[_bidLabel release];
	[_askLabel release];
	[_lastLabel release];
	[_lastChangeLabel release];
	[_lastPercentLabel release];
	[_currencyLabel release];
	[_timeLabel release];
	
    [super dealloc];
}

@end
