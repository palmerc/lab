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
- (CGRect)_buyerSellerLabelFrame;
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
		buyerSeller = [[self createLabel] retain];
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
	[buyerSeller setFrame:[self _buyerSellerLabelFrame]];
	buyerSeller.textAlignment = UITextAlignmentLeft;
}

- (void)setTrade:(Trade *)aTrade {
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
		[doubleFormatter setUsesSignificantDigits:YES];
	}
		
	time.text = aTrade.time;
	price.text = aTrade.price;
	volume.text = aTrade.volume;
	buyerSeller.text = [NSString stringWithFormat:@"%@/%@", aTrade.buyer, aTrade.seller];;
}

- (CGRect)_timeLabelFrame {
	CGSize size = [@"XX:XX:XX" sizeWithFont:mainFont];
	return CGRectMake(0.0f, 0.0, size.width, size.height);
}

- (CGRect)_buyerSellerLabelFrame {
	CGSize size = [@"X" sizeWithFont:mainFont];
	CGFloat width = floorf(self.bounds.size.width / 5.0f);
	return CGRectMake(0.0f + width, 0.0, width, size.height);
}

- (CGRect)_volumeLabelFrame {
	CGSize size = [@"X" sizeWithFont:mainFont];
	CGFloat width = floorf(self.bounds.size.width / 5.0f);
	return CGRectMake(0.0f + width * 2.0f, 0.0, width, size.height);
}

- (CGRect)_priceLabelFrame {
	CGSize size = [@"X" sizeWithFont:mainFont];
	CGFloat width = floorf(self.bounds.size.width / 5.0f);
	
	return CGRectMake(0.0f + width * 3.0f, 0.0, width, size.height);
}

- (void)dealloc {
	[mainFont release];
	[time release];
	[price release];
	[volume release];
	[buyerSeller release];
	
	[trade release];
    
	[super dealloc];
}


@end
