//
//  OrderBookCellP.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "OrderBookTableCellP.h"
#import "Ask.h"
#import "Bid.h"

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
@synthesize mainFont, bid, ask, bidSizeLabel, bidValueLabel, askSizeLabel, askValueLabel;

#pragma mark -
#pragma mark Initialization
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
		self.mainFont = [UIFont boldSystemFontOfSize:17.0];
		self.bidSizeLabel = [self createLabel];		
		self.bidValueLabel = [self createLabel];
		self.askSizeLabel = [self createLabel];
		self.askValueLabel = [self createLabel];
		
		bid = nil;
		ask = nil;
	}
    return self;
}

- (UILabel *)createLabel {	
	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectZero] autorelease];
	[label setBackgroundColor:[UIColor clearColor]];
	[label setFont:mainFont];
	[label setTextColor:[UIColor blackColor]];
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

#define TEXT_LEFT_MARGIN    8.0
#define TEXT_RIGHT_MARGIN   8.0
#define TIME_WIDTH          102.0
#define DESCRIPTION_WIDTH   200.0

/*
 Return the frame of the various subviews -- these are dependent on the editing state of the cell.
 */
- (CGRect)_bidSizeLabelFrame {
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGSize size = [@"X" sizeWithFont:mainFont];
	
	CGFloat width = screenBounds.size.width / 4;
	return CGRectMake(0.0, 0.0, width, size.height);
}

- (CGRect)_bidValueLabelFrame {
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGSize size = [@"X" sizeWithFont:mainFont];
	CGFloat width = screenBounds.size.width / 4;
	
	return CGRectMake(width, 0.0, width, size.height);
}

- (CGRect)_askValueLabelFrame {
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGSize size = [@"X" sizeWithFont:mainFont];
	CGFloat width = screenBounds.size.width / 4;
	return CGRectMake(width * 2, 0.0, width, size.height);
}

- (CGRect)_askSizeLabelFrame {
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGSize size = [@"X" sizeWithFont:mainFont];
	CGFloat width = screenBounds.size.width / 4;
	return CGRectMake(width * 3, 0.0, width, size.height);
}

- (void)drawRect:(CGRect)rect {
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSetRGBFillColor(ctx, 0.0, 0.0, 1.0, 0.5);
	NSLog(@"%f", bid.percent);
	CGContextFillRect(ctx, CGRectMake(0.0, 0.0, bid.percent * 320.0 / 4.0, 50.0));
	
	CGContextSetRGBFillColor(ctx, 1.0, 0.0, 0.0, 0.5);
	NSLog(@"%f", ask.percent);
	CGFloat widthOfLabel = 320.0/4.0;
	CGFloat widthOfRect = ask.percent * widthOfLabel;
	CGFloat askOffset = widthOfLabel - widthOfRect;
	CGContextFillRect(ctx, CGRectMake(3 * widthOfLabel + askOffset, 0.0, ask.percent * 320.0 / 4.0, 50.0));
	
	[super drawRect:rect];
}

- (void)setBid:(Bid *)newBid {
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
		[doubleFormatter setUsesSignificantDigits:YES];
	}
	
	if (newBid != bid) {
		[bid release];
		bid = [newBid retain];
	}
	self.bidSizeLabel.text = [bid.bidSize stringValue];
	self.bidValueLabel.text = [doubleFormatter stringFromNumber:bid.bidValue];
	
}

- (void)setAsk:(Ask *)newAsk {
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
		[doubleFormatter setUsesSignificantDigits:YES];
	}
	
	if (newAsk != ask) {
		[ask release];
		ask = [newAsk retain];
	}
	
	self.askSizeLabel.text = [ask.askSize stringValue];
	self.askValueLabel.text = [doubleFormatter stringFromNumber:ask.askValue];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[bidSizeLabel release];
	[bidValueLabel release];
	[askSizeLabel release];
	[askValueLabel release];
	[mainFont release];
	[ask release];
	[bid release];
    [super dealloc];
}

@end
