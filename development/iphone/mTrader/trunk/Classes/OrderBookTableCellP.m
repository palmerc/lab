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

@interface OrderBookTableCellP ()
- (UILabel *)createLabel;
@end

#pragma mark -
#pragma mark ChainsTableCell implementation
@implementation OrderBookTableCellP
@synthesize bidAsk = _bidAsk;
@synthesize mainFont = _mainFont;

#pragma mark -
#pragma mark Initialization
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
		_maxWidth = 0.0f;
		_mainFont = nil;
		
		_bidSizeLabel = [[self createLabel] retain];		
		_bidValueLabel = [[self createLabel] retain];
		_askSizeLabel = [[self createLabel] retain];
		_askValueLabel = [[self createLabel] retain];
					
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
	
	CGFloat width = floorf(self.bounds.size.width / 4.0f);
	_bidSizeLabel.frame = CGRectMake(0.0, 0.0, width, _lineHeight);
	_bidValueLabel.frame = CGRectMake(width, 0.0, width, _lineHeight);	
	_askValueLabel.frame = CGRectMake(width * 2, 0.0, width, _lineHeight);
	_askSizeLabel.frame = CGRectMake(width * 3, 0.0, width, _lineHeight);
}

- (void)drawRect:(CGRect)rect {
	CGFloat widthOfLabel = floorf(self.bounds.size.width / 4.0f);
	CGFloat askWidth = [_bidAsk.askPercent floatValue] * widthOfLabel;
	CGFloat bidWidth = [_bidAsk.bidPercent floatValue] * widthOfLabel;

	// bid bar
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSetRGBFillColor(ctx, 0.0, 0.0, 1.0, 0.25);
	CGContextFillRect(ctx, CGRectMake(widthOfLabel - bidWidth, 0.0, bidWidth, _lineHeight));
	
	// ask bar
	CGContextSetRGBFillColor(ctx, 1.0, 0.0, 0.0, 0.25);
	CGContextFillRect(ctx, CGRectMake(widthOfLabel * 3, 0.0, askWidth, _lineHeight));
	
	[super drawRect:rect];
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
	
	NSUInteger decimals = [_bidAsk.symbol.feed.decimals integerValue];
	[doubleFormatter setMinimumFractionDigits:decimals];
	[doubleFormatter setMaximumFractionDigits:decimals];
	
	_bidSizeLabel.font = _mainFont;
	_bidValueLabel.font = _mainFont;
	_askSizeLabel.font = _mainFont;
	_askValueLabel.font = _mainFont;
	_bidSizeLabel.text = [_bidAsk.bidSize stringValue];
	_bidValueLabel.text = [doubleFormatter stringFromNumber:_bidAsk.bidPrice];
	_askSizeLabel.text = [_bidAsk.askSize stringValue];
	_askValueLabel.text = [doubleFormatter stringFromNumber:_bidAsk.askPrice];
	
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
