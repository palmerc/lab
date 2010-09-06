//
//  TradesCell.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesCell.h"

#import "Trade.h"

@implementation TradesCell
@synthesize trade = _trade;
@synthesize mainFont = _mainFont;
@synthesize timeLabel = _timeLabel;
@synthesize priceLabel = _priceLabel;
@synthesize volumeLabel = _volumeLabel;
@synthesize buyerLabel = _buyerLabel;
@synthesize sellerLabel = _sellerLabel;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
		_trade = nil;
		_mainFont = nil;
		
		_timeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_timeLabel.textAlignment = UITextAlignmentLeft;
		_timeLabel.textColor = [UIColor darkGrayColor];
		[self.contentView addSubview:_timeLabel];
		
		_priceLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_priceLabel.adjustsFontSizeToFitWidth = YES;
		_priceLabel.textAlignment = UITextAlignmentRight;
		[self.contentView addSubview:_priceLabel];
		
		_volumeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_volumeLabel.adjustsFontSizeToFitWidth = YES;
		_volumeLabel.textAlignment = UITextAlignmentRight;
		[self.contentView addSubview:_volumeLabel];
		
		_buyerLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_buyerLabel.textAlignment = UITextAlignmentCenter;
		[self.contentView addSubview:_buyerLabel];
		
		_sellerLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_sellerLabel.textAlignment = UITextAlignmentCenter;
		[self.contentView addSubview:_sellerLabel];
    }
    return self;
}

- (void)setMainFont:(UIFont *)aFont {
	_timeLabel.font = aFont;
	_priceLabel.font = aFont;
	_volumeLabel.font = aFont;
	_buyerLabel.font = aFont;
	_sellerLabel.font = aFont;
}

- (void)setTrade:(Trade *)aTrade {
	if (_trade == aTrade) {
		return;
	}
	[_trade release];
	_trade = [aTrade retain];	
	
	_timeLabel.text = aTrade.time;
	_priceLabel.text = aTrade.price;
	_volumeLabel.text = aTrade.volume;
	_buyerLabel.text = aTrade.buyer;
	_sellerLabel.text = aTrade.seller;
}

- (void)dealloc {
	[_timeLabel release];
	[_priceLabel release];
	[_volumeLabel release];
	[_buyerLabel release];
	[_sellerLabel release];
	[_mainFont release];
	
	[_trade release];
    
	[super dealloc];
}


@end
