//
//  OtherInfoView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "OtherInfoView.h"

#import "mTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Chart.h"


@implementation OtherInfoView
@synthesize symbol = _symbol;
@synthesize viewController = _viewController;

@synthesize description = _description; 
@synthesize isinLabel = _isinLabel;
@synthesize isin = _isin;
@synthesize segmentLabel = _segmentLabel;
@synthesize segment = _segment;
@synthesize currencyLabel = _currencyLabel;
@synthesize currency = _currency;
@synthesize countryLabel = _countryLabel;
@synthesize country = _country;
@synthesize exchangeLabel = _exchangeLabel;
@synthesize exchange = _exchange;
@synthesize statusLabel = _statusLabel;
@synthesize status = _status;
@synthesize dividendLabel = _dividendLabel;
@synthesize dividend = _dividend;
@synthesize dividendDateLabel = _dividendDateLabel;
@synthesize dividendDate = _dividendDate;
@synthesize sharesLabel = _sharesLabel;
@synthesize shares = _shares;
@synthesize marketCapLabel = _marketCapLabel;
@synthesize marketCap = _marketCap;
@synthesize previousCloseLabel = _previousCloseLabel;
@synthesize previousClose = _previousClose;
@synthesize onVolumeLabel = _onVolumeLabel;
@synthesize onVolume = _onVolume;
@synthesize onValueLabel = _onValueLabel;
@synthesize onValue = _onValue;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self != nil) {
		_description = [[UILabel alloc] initWithFrame:CGRectZero];
		self.description.font = [UIFont boldSystemFontOfSize:17.0];
		self.description.textAlignment = UITextAlignmentCenter;
		[self addSubview:self.description];
		
		_isinLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.isinLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.isinLabel.textAlignment = UITextAlignmentLeft;		
		self.isinLabel.text = NSLocalizedString(@"isinLabel", @"LocalizedString");
		[self addSubview:self.isinLabel];
		
		_isin = [[UILabel alloc] initWithFrame:CGRectZero];
		self.isin.font = [UIFont systemFontOfSize:14.0];	
		self.isin.textAlignment = UITextAlignmentRight;		
		[self addSubview:self.isin];
		
		_segmentLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.segmentLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.segmentLabel.textAlignment = UITextAlignmentLeft;		
		self.segmentLabel.text = NSLocalizedString(@"segmentLabel", @"LocalizedString");
		[self addSubview:self.segmentLabel];
		
		_segment = [[UILabel alloc] initWithFrame:CGRectZero];
		self.segment.font = [UIFont systemFontOfSize:14.0];	
		self.segment.textAlignment = UITextAlignmentRight;		
		[self addSubview:self.segment];
		
		_currencyLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.currencyLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.currencyLabel.textAlignment = UITextAlignmentLeft;	
		self.currencyLabel.text = NSLocalizedString(@"currencyLabel", @"LocalizedString");
		[self addSubview:self.currencyLabel];
		
		_currency = [[UILabel alloc] initWithFrame:CGRectZero];
		self.currency.font = [UIFont systemFontOfSize:14.0];	
		self.currency.textAlignment = UITextAlignmentRight;		
		[self addSubview:self.currency];
		
		_countryLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.countryLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.countryLabel.textAlignment = UITextAlignmentLeft;	
		self.countryLabel.text = NSLocalizedString(@"countryLabel", @"LocalizedString");
		[self addSubview:self.countryLabel];
		
		_country = [[UILabel alloc] initWithFrame:CGRectZero];
		self.country.font = [UIFont systemFontOfSize:14.0];	
		self.country.textAlignment = UITextAlignmentRight;		
		[self addSubview:self.country];
		
		_exchangeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.exchangeLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.exchangeLabel.textAlignment = UITextAlignmentLeft;		
		self.exchangeLabel.text = NSLocalizedString(@"exchangeLabel", @"LocalizedString");
		[self addSubview:self.exchangeLabel];
		
		_exchange = [[UILabel alloc] initWithFrame:CGRectZero];
		self.exchange.font = [UIFont systemFontOfSize:14.0];
		self.exchange.textAlignment = UITextAlignmentRight;
		[self addSubview:self.exchange];
		
		_statusLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.statusLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.statusLabel.textAlignment = UITextAlignmentLeft;	
		self.statusLabel.text = NSLocalizedString(@"statusLabel", @"LocalizedString");
		[self addSubview:self.statusLabel];
		
		_status = [[UILabel alloc] initWithFrame:CGRectZero];
		self.status.font = [UIFont systemFontOfSize:14.0];
		self.status.textAlignment = UITextAlignmentRight;
		[self addSubview:self.status];
		
		_dividendLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.dividendLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.dividendLabel.textAlignment = UITextAlignmentLeft;
		self.dividendLabel.text = NSLocalizedString(@"dividendLabel", @"LocalizedString");	
		[self addSubview:self.dividendLabel];
		
		_dividend = [[UILabel alloc] initWithFrame:CGRectZero];
		self.dividend.font = [UIFont systemFontOfSize:14.0];	
		self.dividend.textAlignment = UITextAlignmentRight;	
		[self addSubview:self.dividend];
		
		_dividendDateLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.dividendDateLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.dividendDateLabel.textAlignment = UITextAlignmentLeft;
		self.dividendDateLabel.text = NSLocalizedString(@"dividendDateLabel", @"LocalizedString");
		[self addSubview:self.dividendDateLabel];
		
		_dividendDate = [[UILabel alloc] initWithFrame:CGRectZero];
		self.dividendDate.font = [UIFont systemFontOfSize:14.0];	
		self.dividendDate.textAlignment = UITextAlignmentRight;		
		[self addSubview:self.dividendDate];
		
		_sharesLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.sharesLabel.font = [UIFont boldSystemFontOfSize:14.0];	
		self.sharesLabel.textAlignment = UITextAlignmentLeft;	
		self.sharesLabel.text = NSLocalizedString(@"sharesLabel", @"LocalizedString");	
		[self addSubview:self.sharesLabel];
		
		_shares = [[UILabel alloc] initWithFrame:CGRectZero];
		self.shares.font = [UIFont systemFontOfSize:14.0];
		self.shares.textAlignment = UITextAlignmentRight;
		[self addSubview:self.shares];
		
		_marketCapLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.marketCapLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.marketCapLabel.textAlignment = UITextAlignmentLeft;
		self.marketCapLabel.text = NSLocalizedString(@"marketCapLabel", @"LocalizedString");
		[self addSubview:self.marketCapLabel];
		
		_marketCap = [[UILabel alloc] initWithFrame:CGRectZero];
		self.marketCap.font = [UIFont systemFontOfSize:14.0];
		self.marketCap.textAlignment = UITextAlignmentRight;
		[self addSubview:self.marketCap];
		
		_previousCloseLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.previousCloseLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.previousCloseLabel.textAlignment = UITextAlignmentLeft;
		self.previousCloseLabel.text = NSLocalizedString(@"previousCloseLabel", @"LocalizedString");
		[self addSubview:self.previousCloseLabel];
		
		_previousClose = [[UILabel alloc] initWithFrame:CGRectZero];
		self.previousClose.font = [UIFont systemFontOfSize:14.0];
		self.previousClose.textAlignment = UITextAlignmentRight;
		[self addSubview:self.previousClose];
		
		_onVolumeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.onVolumeLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.onVolumeLabel.textAlignment = UITextAlignmentLeft;
		self.onVolumeLabel.text = NSLocalizedString(@"onVolumeLabel", @"LocalizedString");
		[self addSubview:self.onVolumeLabel];
		
		_onVolume = [[UILabel alloc] initWithFrame:CGRectZero];
		self.onVolume.font = [UIFont systemFontOfSize:14.0];
		self.onVolume.textAlignment = UITextAlignmentRight;
		[self addSubview:self.onVolume];
		
		_onValueLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.onValueLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.onValueLabel.textAlignment = UITextAlignmentLeft;
		self.onValueLabel.text = NSLocalizedString(@"onValueLabel", @"LocalizedString");
		[self addSubview:self.onValueLabel];
		
		_onValue = [[UILabel alloc] initWithFrame:CGRectZero];
		self.onValue.font = [UIFont systemFontOfSize:14.0];
		self.onValue.textAlignment = UITextAlignmentRight;
		[self addSubview:self.onValue];
	}
	return self;
}

#pragma mark -
#pragma mark UIView drawing
- (void)drawRect:(CGRect)rect {
//	super.padding = 6.0f;
//	super.cornerRadius = 10.0f;
//	super.strokeWidth = 0.75f;
//	[super drawRect:rect];
//	
//	CGFloat leftPadding = ceilf(self.padding + super.strokeWidth + kBlur);
//	CGFloat maxWidth = rect.size.width - leftPadding * 2.0f;
//	
//	UIFont *labelFont = [UIFont boldSystemFontOfSize:14.0];
//	
//	CGFloat globalY = leftPadding;
//	
//	CGSize descriptionSize = [self.description.text sizeWithFont:labelFont];
//	self.description.frame = CGRectMake(leftPadding, globalY, maxWidth, descriptionSize.height);
//	globalY += descriptionSize.height; 
//	
//	CGSize isinLabelSize = [self.isinLabel.text sizeWithFont:labelFont];
//	self.isinLabel.frame = CGRectMake(leftPadding, globalY, isinLabelSize.width, isinLabelSize.height);
//	
//	CGSize isinSize = [self.isin.text sizeWithFont:labelFont];
//	self.isin.frame = CGRectMake(leftPadding + isinLabelSize.width, globalY, maxWidth - isinLabelSize.width, isinSize.height);
//	globalY += isinSize.height;
//	
//	CGSize segmentLabelSize = [self.segmentLabel.text sizeWithFont:labelFont];
//	self.segmentLabel.frame = CGRectMake(leftPadding, globalY, segmentLabelSize.width, segmentLabelSize.height);
//	
//	CGSize segmentSize = [self.segment.text sizeWithFont:labelFont];
//	self.segment.frame = CGRectMake(leftPadding + segmentLabelSize.width, globalY, maxWidth - segmentLabelSize.width, segmentSize.height);
//	globalY += segmentSize.height;
//	
//	CGSize currencyLabelSize = [self.currencyLabel.text sizeWithFont:labelFont];
//	self.currencyLabel.frame = CGRectMake(leftPadding, globalY, currencyLabelSize.width, currencyLabelSize.height);
//	
//	CGSize currencySize = [self.currency.text sizeWithFont:labelFont];
//	self.currency.frame = CGRectMake(leftPadding + currencyLabelSize.width, globalY, maxWidth - currencyLabelSize.width, currencySize.height);
//	globalY += currencySize.height;
//	
//	CGSize countryLabelSize = [self.countryLabel.text sizeWithFont:labelFont];
//	self.countryLabel.frame = CGRectMake(leftPadding, globalY, countryLabelSize.width, countryLabelSize.height);
//	
//	CGSize countrySize = [self.country.text sizeWithFont:labelFont];
//	self.country.frame = CGRectMake(leftPadding + countryLabelSize.width, globalY, maxWidth - countryLabelSize.width, countrySize.height);
//	globalY += countrySize.height;
//	
//	CGSize exchangeLabelSize = [self.exchangeLabel.text sizeWithFont:labelFont];
//	self.exchangeLabel.frame = CGRectMake(leftPadding, globalY, exchangeLabelSize.width, exchangeLabelSize.height);
//	
//	CGSize exchangeSize = [self.exchange.text sizeWithFont:labelFont];
//	self.exchange.frame = CGRectMake(leftPadding + exchangeLabelSize.width, globalY, maxWidth - exchangeLabelSize.width, exchangeSize.height);
//	globalY += exchangeSize.height;
//	
//	CGSize statusLabelSize = [self.statusLabel.text sizeWithFont:labelFont];
//	self.statusLabel.frame = CGRectMake(leftPadding, globalY, statusLabelSize.width, statusLabelSize.height);
//	
//	CGSize statusSize = [self.status.text sizeWithFont:labelFont];
//	self.status.frame = CGRectMake(leftPadding + statusLabelSize.width, globalY, maxWidth - statusLabelSize.width, statusSize.height);
}

#pragma mark -
#pragma mark Symbol methods
- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", self.symbol.feed.feedNumber, self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] staticDataForFeedTicker:feedTicker];
	
	[self.symbol addObserver:self forKeyPath:@"symbolDynamicData.lastTrade" options:NSKeyValueObservingOptionNew context:nil];
	
	[self updateSymbol];
	[self setNeedsDisplay];
}

- (void)updateSymbol {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateStyle:NSDateFormatterMediumStyle];
	}
	
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	}

	self.description.text = self.symbol.companyName; 
	self.isin.text = self.symbol.isin;
	self.segment.text = self.symbol.symbolDynamicData.segment;
	self.currency.text = self.symbol.currency;
	self.country.text = self.symbol.country;
	self.exchange.text = self.symbol.feed.feedName;
	self.status.text = self.symbol.symbolDynamicData.tradingStatus;
	self.dividend.text = [self.symbol.symbolDynamicData.dividend stringValue];
	self.dividendDate.text = [dateFormatter stringFromDate:self.symbol.symbolDynamicData.dividendDate];
	self.shares.text = self.symbol.symbolDynamicData.outstandingShares;
	self.marketCap.text = self.symbol.symbolDynamicData.marketCapitalization;
	self.previousClose.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.previousClose];
	self.onVolume.text = self.symbol.symbolDynamicData.onVolume;
	self.onValue.text = self.symbol.symbolDynamicData.onValue;
}

#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"symbolDynamicData.lastTrade"]) {
		[self updateSymbol];
	}
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[self.symbol removeObserver:self forKeyPath:@"symbolDynamicData.lastTrade"];
	
	[_symbol release];
	[_description release]; 
	[_isinLabel release];
	[_isin release];
	[_segmentLabel release];
	[_segment release];
	[_currencyLabel release];
	[_currency release];
	[_countryLabel release];
	[_country release];
	[_exchangeLabel release];
	[_exchange release];
	[_statusLabel release];
	[_status release];
	[_dividendLabel release];
	[_dividend release];
	[_dividendDateLabel release];
	[_dividendDate release];
	[_sharesLabel release];
	[_shares release];
	[_marketCapLabel release];
	[_marketCap release];
	[_previousCloseLabel release];
	[_previousClose release];
	[_onVolumeLabel release];
	[_onVolume release];
	[_onValueLabel release];
	[_onValue release];
	
	
	[super dealloc];
}

@end
