//
//  OrderBookCellP.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "OrderBookTableCellP.h"
#import "BidAsk.h"
#import "Feed.h"
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
@synthesize bidAsk = _bidAsk;
@synthesize maxWidth = _maxWidth;
@synthesize mainFont = _mainFont;
@synthesize bidSizeLabel = _bidSizeLabel;
@synthesize bidValueLabel = _bidValueLabel;
@synthesize askSizeLabel = _askSizeLabel;
@synthesize askValueLabel = _askValueLabel;

#pragma mark -
#pragma mark Initialization
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
		_maxWidth = 0.0f;
		_mainFont = nil;
		
		self.bidSizeLabel = [self createLabel];		
		self.bidValueLabel = [self createLabel];
		self.askSizeLabel = [self createLabel];
		self.askValueLabel = [self createLabel];
					
		_bidAsk = nil;
		
	}
    return self;
}

- (void)setMainFont:(UIFont *)font {
	if (_mainFont != nil) {
		[_mainFont release];
	}
	_mainFont = [font retain];
	CGSize fontSize = [@"X" sizeWithFont:font];
	_lineHeight = fontSize.height;
	
	[self setNeedsLayout];
}

- (UILabel *)createLabel {	
	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectZero] autorelease];
	[label setBackgroundColor:[UIColor clearColor]];
	[label setTextColor:[UIColor blackColor]];
	[label setAdjustsFontSizeToFitWidth:YES];
	[label setTextAlignment:UITextAlignmentRight];
	[label setHighlightedTextColor:[UIColor blackColor]];
	[self.contentView addSubview:label];
	
	return label;
}

#pragma mark -
#pragma mark Laying out subviews

/*
 To save space, the prep time label disappears during editing.
 */
- (void)layoutSubviews {
    [super layoutSubviews];
	
    [self.bidSizeLabel setFrame:[self _bidSizeLabelFrame]];
	[self.bidValueLabel setFrame:[self _bidValueLabelFrame]];
	[self.askSizeLabel setFrame:[self _askSizeLabelFrame]];
	[self.askValueLabel setFrame:[self _askValueLabelFrame]];

}

/*
 Return the frame of the various subviews -- these are dependent on the editing state of the cell.
 */
- (CGRect)_bidSizeLabelFrame {
	CGFloat width = floorf(self.maxWidth / 4.0f);
	return CGRectMake(0.0, 0.0, width, _lineHeight);
}

- (CGRect)_bidValueLabelFrame {
	CGFloat width = floorf(self.maxWidth / 4.0f);
	return CGRectMake(width, 0.0, width, _lineHeight);
}

- (CGRect)_askValueLabelFrame {
	CGFloat width = floorf(self.maxWidth / 4.0f);
	return CGRectMake(width * 2, 0.0, width, _lineHeight);
}

- (CGRect)_askSizeLabelFrame {
	CGFloat width = floorf(self.maxWidth / 4.0f);
	return CGRectMake(width * 3, 0.0, width, _lineHeight);
}

- (void)drawRect:(CGRect)rect {
	CGFloat widthOfLabel = floorf(self.maxWidth / 4.0f);
	CGFloat askWidth = [self.bidAsk.askPercent floatValue] * widthOfLabel;
	CGFloat bidWidth = [self.bidAsk.bidPercent floatValue] * widthOfLabel;

	// bid bar
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSetRGBFillColor(ctx, 0.0, 0.0, 1.0, 0.25);
	CGContextFillRect(ctx, CGRectMake(widthOfLabel - bidWidth, 0.0, bidWidth, _lineHeight));
	
	// ask bar
	CGContextSetRGBFillColor(ctx, 1.0, 0.0, 0.0, 0.25);
	CGContextFillRect(ctx, CGRectMake(widthOfLabel * 3, 0.0, askWidth, _lineHeight));
	
	[super drawRect:rect];
}

- (void)setMaxWidth:(CGFloat)width {
	_maxWidth = width;
	[self setNeedsLayout];
	[self setNeedsDisplay];
}

- (void)setBidAsk:(BidAsk *)newBidAsk {
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	}
	
	if (newBidAsk != self.bidAsk) {
		[_bidAsk release];
		_bidAsk = [newBidAsk retain];
	}
	
	NSUInteger decimals = [self.bidAsk.symbol.feed.decimals integerValue];
	[doubleFormatter setMinimumFractionDigits:decimals];
	[doubleFormatter setMaximumFractionDigits:decimals];
	
	self.bidSizeLabel.font = self.mainFont;
	self.bidValueLabel.font = self.mainFont;
	self.askSizeLabel.font = self.mainFont;
	self.askValueLabel.font = self.mainFont;
	self.bidSizeLabel.text = [self.bidAsk.bidSize stringValue];
	self.bidValueLabel.text = [doubleFormatter stringFromNumber:self.bidAsk.bidPrice];
	self.askSizeLabel.text = [self.bidAsk.askSize stringValue];
	self.askValueLabel.text = [doubleFormatter stringFromNumber:self.bidAsk.askPrice];
	
	[self setNeedsDisplay];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_bidSizeLabel release];
	[_bidValueLabel release];
	[_askSizeLabel release];
	[_askValueLabel release];
	
	[_mainFont release];
	[_bidAsk release];
    [super dealloc];
}

@end
