//
//  OrderBookView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010  AS. All rights reserved.
//

#define DEBUG 0

#import "OrderBookView.h"

@implementation OrderBookView
@synthesize orderbookAvailableLabel = _orderbookAvailableLabel;
@synthesize tableView = _tableView;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {	
		_headerFont = [UIFont boldSystemFontOfSize:14.0];
		
		_orderBookButton = [[UIButton alloc] initWithFrame:frame];
		
		_askSizeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_askSizeLabel.textAlignment = UITextAlignmentCenter;
		_askSizeLabel.font = _headerFont;
		_askSizeLabel.text = NSLocalizedString(@"askSize", @"A Size");
		[_orderBookButton addSubview:_askSizeLabel];

		_askValueLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_askValueLabel.textAlignment = UITextAlignmentCenter;
		_askValueLabel.font = _headerFont;
		_askValueLabel.text = NSLocalizedString(@"askPrice", @"A Price");
		[_orderBookButton addSubview:_askValueLabel];
		
		_bidSizeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_bidSizeLabel.textAlignment = UITextAlignmentCenter;
		_bidSizeLabel.font = _headerFont;
		_bidSizeLabel.text = NSLocalizedString(@"bidSize", @"B Size");
		[_orderBookButton addSubview:_bidSizeLabel];

		_bidValueLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_bidValueLabel.textAlignment = UITextAlignmentCenter;
		_bidValueLabel.font = _headerFont;
		_bidValueLabel.text = NSLocalizedString(@"bidPrice", @"B Price");
		[_orderBookButton addSubview:_bidValueLabel];
		
		NSString *labelString = NSLocalizedString(@"noOrderbookAvailable", @"No Orderbook Available");
		
		_orderbookAvailableLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_orderbookAvailableLabel.textAlignment = UITextAlignmentCenter;
		_orderbookAvailableLabel.font = _headerFont;
		_orderbookAvailableLabel.textColor = [UIColor blackColor];
		_orderbookAvailableLabel.backgroundColor = [UIColor whiteColor];
		_orderbookAvailableLabel.text = labelString;
		_orderbookAvailableLabel.hidden = YES;
		[_orderBookButton addSubview:_orderbookAvailableLabel];

		_tableView = [[UITableView alloc] initWithFrame:CGRectZero];
		[_orderBookButton addSubview:_tableView];
		
		[self addSubview:_orderBookButton];
	}
	
    return self;
}

- (void)layoutSubviews {
	CGRect bounds = self.bounds;
	
	CGSize headerFontSize = [@"X" sizeWithFont:_headerFont];
	CGFloat quarterWidth = floorf(bounds.size.width / 4.0f);
	CGRect bidSizeLabelFrame = CGRectMake(0.0f, 0.0f, quarterWidth, headerFontSize.height);
	CGRect bidValueLabelFrame = CGRectMake(quarterWidth, 0.0f, quarterWidth, headerFontSize.height);
	CGRect askValueLabelFrame = CGRectMake(quarterWidth * 2.0f, 0.0f, quarterWidth, headerFontSize.height);
	CGRect askSizeLabelFrame = CGRectMake(quarterWidth * 3.0f, 0.0f, quarterWidth, headerFontSize.height);
	
	CGRect tableViewFrame = CGRectMake(0.0f, headerFontSize.height, bounds.size.width, bounds.size.height - headerFontSize.height);

	_bidSizeLabel.frame = bidSizeLabelFrame;
	_bidValueLabel.frame = bidValueLabelFrame;
	_askValueLabel.frame = askValueLabelFrame;
	_askSizeLabel.frame = askSizeLabelFrame;
	_tableView.frame = tableViewFrame;
	_orderbookAvailableLabel.frame = CGRectMake(0.0f, 0.0f, bounds.size.width, headerFontSize.height);
	
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_headerFont release];
	[_askSizeLabel release];
	[_askValueLabel release];
	[_bidValueLabel release];
	[_bidSizeLabel release];
	[_orderbookAvailableLabel release];
	[_orderBookButton release];
		
    [super dealloc];
}

@end
