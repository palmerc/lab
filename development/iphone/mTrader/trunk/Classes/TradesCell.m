//
//  TradesCell.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesCell.h"

#import "Trade.h"

#pragma mark -
#pragma mark SubviewFrames category

@interface TradesCell (SubviewFrames)
- (CGRect)_timeLabelFrame;
- (CGRect)_priceLabelFrame;
- (CGRect)_volumeLabelFrame;
@end

@implementation TradesCell
@synthesize trade;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
		trade = nil;
		
		mainFont = [[UIFont systemFontOfSize:17.0] retain];
		time = [[self createLabel] retain];
		time.textAlignment = UITextAlignmentCenter;
		price = [[self createLabel] retain];
		volume = [[self createLabel] retain];
    }
    return self;
}

- (UILabel *)createLabel {
	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectZero] autorelease];
	label.textAlignment = UITextAlignmentRight;
	[self.contentView addSubview:label];
	
	return label;
}

#pragma mark -
#pragma mark Laying out subviews

- (void)layoutSubviews {
    [super layoutSubviews];
	
    [time setFrame:[self _timeLabelFrame]];
	[price setFrame:[self _priceLabelFrame]];
	[volume setFrame:[self _volumeLabelFrame]];
}

- (void)setTrade:(Trade *)aTrade {
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
		[doubleFormatter setUsesSignificantDigits:YES];
	}
		
	time.text = aTrade.time;
	price.text = [doubleFormatter stringFromNumber:aTrade.price];
	volume.text = [doubleFormatter stringFromNumber:aTrade.volume];
}

- (CGRect)_timeLabelFrame {
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGSize size = [@"X" sizeWithFont:mainFont];
	
	CGFloat width = screenBounds.size.width / 3;
	return CGRectMake(8.0f, 0.0, width - 10.0f, size.height);
}

- (CGRect)_priceLabelFrame {
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGSize size = [@"X" sizeWithFont:mainFont];
	CGFloat width = screenBounds.size.width / 3;
	
	return CGRectMake(width, 0.0, width - 6.0f, size.height);
}

- (CGRect)_volumeLabelFrame {
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGSize size = [@"X" sizeWithFont:mainFont];
	CGFloat width = screenBounds.size.width / 3;
	return CGRectMake(width * 2, 0.0, width - 10.0f, size.height);
}

- (void)dealloc {
	[mainFont release];
	[time release];
	[price release];
	[volume release];
	
	[trade release];
    
	[super dealloc];
}


@end
