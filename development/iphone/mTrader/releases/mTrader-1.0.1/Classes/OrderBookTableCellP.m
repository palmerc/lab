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
@synthesize maxWidth, mainFont, bidSizeLabel, bidValueLabel, askSizeLabel, askValueLabel;

#pragma mark -
#pragma mark Initialization
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
		maxWidth = 0.0f;
		mainFont = nil;
		bidSizeLabel = [self createLabel];		
		bidValueLabel = [self createLabel];
		askSizeLabel = [self createLabel];
		askValueLabel = [self createLabel];
					
		_bidAsk = nil;
		
	}
    return self;
}

- (void)setMainFont:(UIFont *)font {
	if (mainFont != nil) {
		[mainFont release];
	}
	mainFont = [font retain];
	CGSize fontSize = [@"X" sizeWithFont:font];
	lineHeight = fontSize.height;
	
	[self setNeedsDisplay];
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
	
    [bidSizeLabel setFrame:[self _bidSizeLabelFrame]];
	[bidValueLabel setFrame:[self _bidValueLabelFrame]];
	[askSizeLabel setFrame:[self _askSizeLabelFrame]];
	[askValueLabel setFrame:[self _askValueLabelFrame]];

}

/*
 Return the frame of the various subviews -- these are dependent on the editing state of the cell.
 */
- (CGRect)_bidSizeLabelFrame {
	CGFloat width = floorf(self.maxWidth / 4.0f);
	return CGRectMake(0.0, 0.0, width, lineHeight);
}

- (CGRect)_bidValueLabelFrame {
	CGFloat width = floorf(self.maxWidth / 4.0f);
	return CGRectMake(width, 0.0, width, lineHeight);
}

- (CGRect)_askValueLabelFrame {
	CGFloat width = floorf(self.maxWidth / 4.0f);
	return CGRectMake(width * 2, 0.0, width, lineHeight);
}

- (CGRect)_askSizeLabelFrame {
	CGFloat width = floorf(self.maxWidth / 4.0f);
	return CGRectMake(width * 3, 0.0, width, lineHeight);
}

- (void)drawRect:(CGRect)rect {
	CGFloat widthOfLabel = floorf(self.maxWidth / 4.0f);
	CGFloat askWidth = [self.bidAsk.askPercent floatValue] * widthOfLabel;
	CGFloat bidWidth = [self.bidAsk.bidPercent floatValue] * widthOfLabel;

	// bid bar
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSetRGBFillColor(ctx, 0.0, 0.0, 1.0, 0.25);
	CGContextFillRect(ctx, CGRectMake(widthOfLabel - bidWidth, 0.0, bidWidth, lineHeight));
	
	// ask bar
	CGContextSetRGBFillColor(ctx, 1.0, 0.0, 0.0, 0.25);
	CGContextFillRect(ctx, CGRectMake(widthOfLabel * 3, 0.0, askWidth, lineHeight));
	
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
	
	NSUInteger decimals = [self.bidAsk.symbol.feed.decimals integerValue];
	[doubleFormatter setMinimumFractionDigits:decimals];
	[doubleFormatter setMaximumFractionDigits:decimals];
	
	self.bidSizeLabel.font = mainFont;
	self.bidValueLabel.font = mainFont;
	self.askSizeLabel.font = mainFont;
	self.askValueLabel.font = mainFont;
	self.bidSizeLabel.text = [self.bidAsk.bidSize stringValue];
	self.bidValueLabel.text = [doubleFormatter stringFromNumber:self.bidAsk.bidPrice];
	self.askSizeLabel.text = [self.bidAsk.askSize stringValue];
	self.askValueLabel.text = [doubleFormatter stringFromNumber:self.bidAsk.askPrice];
	
	[self setNeedsDisplay];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[bidSizeLabel release];
	[bidValueLabel release];
	[askSizeLabel release];
	[askValueLabel release];
	[mainFont release];
	[_bidAsk release];
    [super dealloc];
}

@end
