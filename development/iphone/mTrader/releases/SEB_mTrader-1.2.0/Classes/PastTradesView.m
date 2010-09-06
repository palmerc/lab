//
//  PastTradesView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 17.08.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "PastTradesView.h"


@implementation PastTradesView
@synthesize timeLabel = _timeLabel;
@synthesize priceLabel = _priceLabel;
@synthesize volumeLabel = _volumeLabel;
@synthesize buyerLabel = _buyerLabel;
@synthesize sellerLabel = _sellerLabel;
@synthesize tableView = _tableView;
@synthesize tradesAvailableLabel = _tradesAvailableLabel;
@synthesize modal = _modal;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
        // Initialization code
		_modal = NO;
		
		_headerFont = [[UIFont boldSystemFontOfSize:14.0] retain];
		
		_timeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_timeLabel.textAlignment = UITextAlignmentCenter;
		_timeLabel.font = _headerFont;
		_timeLabel.text = NSLocalizedString(@"time", @"Time");
		[self addSubview:_timeLabel];	
		
		_priceLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_priceLabel.textAlignment = UITextAlignmentCenter;
		_priceLabel.font = _headerFont;
		_priceLabel.text = NSLocalizedString(@"price", @"Price");
		[self addSubview:_priceLabel];
		
		_volumeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_volumeLabel.textAlignment = UITextAlignmentCenter;
		_volumeLabel.font = _headerFont;
		_volumeLabel.text = NSLocalizedString(@"volume", @"Volume");
		[self addSubview:_volumeLabel];
		
		_buyerLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_buyerLabel.textAlignment = UITextAlignmentCenter;
		_buyerLabel.font = _headerFont;
		_buyerLabel.text = NSLocalizedString(@"buyer", @"Buyer");
		[self addSubview:_buyerLabel];
		
		_sellerLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_sellerLabel.textAlignment = UITextAlignmentCenter;
		_sellerLabel.font = _headerFont;
		_sellerLabel.text = NSLocalizedString(@"seller", @"Seller");
		[self addSubview:_sellerLabel];
		
		_tableView = [[UITableView alloc] initWithFrame:CGRectZero];
		[self addSubview:_tableView];
		
		_tradesAvailableLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_tradesAvailableLabel.textAlignment = UITextAlignmentCenter;
		_tradesAvailableLabel.font = _headerFont;
		_tradesAvailableLabel.textColor = [UIColor blackColor];
		_tradesAvailableLabel.backgroundColor = [UIColor clearColor];
		_tradesAvailableLabel.text = NSLocalizedString(@"noTradesDataAvailable", @"No Trades Data Available");
		_tradesAvailableLabel.hidden = YES;
		
    }
    return self;
}

- (void)layoutSubviews {
	CGSize headerFontSize = [@"X" sizeWithFont:_headerFont];
	
	CGRect viewBounds = self.bounds;
	CGFloat fifthWidth = floorf(viewBounds.size.width / 5.0f);
	CGRect timeLabelFrame = CGRectMake(0.0f, 0.0f, fifthWidth, headerFontSize.height);
	CGRect priceLabelFrame = CGRectMake(fifthWidth, 0.0f, fifthWidth, headerFontSize.height);
	CGRect volumeLabelFrame = CGRectMake(fifthWidth * 2.0f, 0.0f, fifthWidth, headerFontSize.height);
	CGRect buyerLabelFrame = CGRectMake(fifthWidth * 3.0f, 0.0f, fifthWidth, headerFontSize.height);
	CGRect sellerLabelFrame = CGRectMake(fifthWidth * 4.0f, 0.0f, fifthWidth, headerFontSize.height);
	CGRect tableViewFrame = CGRectMake(viewBounds.origin.x, headerFontSize.height, viewBounds.size.width, viewBounds.size.height - headerFontSize.height);
	
	CGRect tradesAvailableFrame = CGRectMake(0.0f, 0.0f, viewBounds.size.width, headerFontSize.height);
	
	_timeLabel.frame = timeLabelFrame;
	_priceLabel.frame = priceLabelFrame;
	_volumeLabel.frame = volumeLabelFrame;
	_buyerLabel.frame = buyerLabelFrame;
	_sellerLabel.frame = sellerLabelFrame;
	_tableView.frame = tableViewFrame;
	_tradesAvailableLabel.frame = tradesAvailableFrame;
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
	if (_modal) {
		return [super hitTest:point withEvent:event];
	} else {
		return nil;
	}
}

- (void)dealloc {
	[_headerFont release];	
	[_timeLabel release];
	[_priceLabel release];
	[_volumeLabel release];
	[_buyerLabel release];
	[_sellerLabel release];
	[_tableView release];
	[_tradesAvailableLabel release];
	
    [super dealloc];
}


@end
