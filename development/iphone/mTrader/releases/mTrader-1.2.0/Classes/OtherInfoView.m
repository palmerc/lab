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

@interface OtherInfoView ()
- (void)updateSymbol;
@end


@implementation OtherInfoView
@synthesize symbol = _symbol;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self != nil) {
		
		UIFont *boldDescription = [UIFont boldSystemFontOfSize:14.0f];
		UIFont *regularDescription = [UIFont systemFontOfSize:14.0f];
		
		_description = [[UILabel alloc] initWithFrame:CGRectZero];
		_description.font = boldDescription;
		_description.textAlignment = UITextAlignmentCenter;
		[self addSubview:_description];
		
		_isinLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_isinLabel.font = boldDescription;
		_isinLabel.textAlignment = UITextAlignmentLeft;		
		_isinLabel.text = NSLocalizedString(@"isinLabel", @"LocalizedString");
		[self addSubview:_isinLabel];
		
		_isin = [[UILabel alloc] initWithFrame:CGRectZero];
		_isin.font = regularDescription;	
		_isin.textAlignment = UITextAlignmentRight;		
		[self addSubview:_isin];
		
		_segmentLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_segmentLabel.font = boldDescription;
		_segmentLabel.textAlignment = UITextAlignmentLeft;		
		_segmentLabel.text = NSLocalizedString(@"segmentLabel", @"LocalizedString");
		[self addSubview:_segmentLabel];
		
		_segment = [[UILabel alloc] initWithFrame:CGRectZero];
		_segment.font = regularDescription;	
		_segment.textAlignment = UITextAlignmentRight;		
		[self addSubview:_segment];
		
		_currencyLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_currencyLabel.font = boldDescription;
		_currencyLabel.textAlignment = UITextAlignmentLeft;	
		_currencyLabel.text = NSLocalizedString(@"currencyLabel", @"LocalizedString");
		[self addSubview:_currencyLabel];
		
		_currency = [[UILabel alloc] initWithFrame:CGRectZero];
		_currency.font = regularDescription;	
		_currency.textAlignment = UITextAlignmentRight;		
		[self addSubview:_currency];
		
		_countryLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_countryLabel.font = boldDescription;
		_countryLabel.textAlignment = UITextAlignmentLeft;	
		_countryLabel.text = NSLocalizedString(@"countryLabel", @"LocalizedString");
		[self addSubview:_countryLabel];
		
		_country = [[UILabel alloc] initWithFrame:CGRectZero];
		_country.font = regularDescription;	
		_country.textAlignment = UITextAlignmentRight;		
		[self addSubview:_country];
		
		_exchangeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_exchangeLabel.font = boldDescription;
		_exchangeLabel.textAlignment = UITextAlignmentLeft;		
		_exchangeLabel.text = NSLocalizedString(@"exchangeLabel", @"LocalizedString");
		[self addSubview:_exchangeLabel];
		
		_exchange = [[UILabel alloc] initWithFrame:CGRectZero];
		_exchange.font = regularDescription;
		_exchange.textAlignment = UITextAlignmentRight;
		[self addSubview:_exchange];
		
		_statusLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_statusLabel.font = boldDescription;
		_statusLabel.textAlignment = UITextAlignmentLeft;	
		_statusLabel.text = NSLocalizedString(@"statusLabel", @"LocalizedString");
		[self addSubview:_statusLabel];
		
		_status = [[UILabel alloc] initWithFrame:CGRectZero];
		_status.font = regularDescription;
		_status.textAlignment = UITextAlignmentRight;
		[self addSubview:_status];
		
		_dividendLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_dividendLabel.font = boldDescription;
		_dividendLabel.textAlignment = UITextAlignmentLeft;
		_dividendLabel.text = NSLocalizedString(@"dividendLabel", @"LocalizedString");	
		[self addSubview:_dividendLabel];
		
		_dividend = [[UILabel alloc] initWithFrame:CGRectZero];
		_dividend.font = regularDescription;	
		_dividend.textAlignment = UITextAlignmentRight;	
		[self addSubview:_dividend];
		
		_dividendDateLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_dividendDateLabel.font = boldDescription;
		_dividendDateLabel.textAlignment = UITextAlignmentLeft;
		_dividendDateLabel.text = NSLocalizedString(@"dividendDateLabel", @"LocalizedString");
		[self addSubview:_dividendDateLabel];
		
		_dividendDate = [[UILabel alloc] initWithFrame:CGRectZero];
		_dividendDate.font = regularDescription;	
		_dividendDate.textAlignment = UITextAlignmentRight;		
		[self addSubview:_dividendDate];
		
		_sharesLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_sharesLabel.font = boldDescription;	
		_sharesLabel.textAlignment = UITextAlignmentLeft;	
		_sharesLabel.text = NSLocalizedString(@"sharesLabel", @"LocalizedString");	
		[self addSubview:_sharesLabel];
		
		_shares = [[UILabel alloc] initWithFrame:CGRectZero];
		_shares.font = regularDescription;
		_shares.textAlignment = UITextAlignmentRight;
		[self addSubview:_shares];
		
		_marketCapLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_marketCapLabel.font = boldDescription;
		_marketCapLabel.textAlignment = UITextAlignmentLeft;
		_marketCapLabel.text = NSLocalizedString(@"marketCapLabel", @"LocalizedString");
		[self addSubview:_marketCapLabel];
		
		_marketCap = [[UILabel alloc] initWithFrame:CGRectZero];
		_marketCap.font = regularDescription;
		_marketCap.textAlignment = UITextAlignmentRight;
		[self addSubview:_marketCap];
		
		_previousCloseLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_previousCloseLabel.font = boldDescription;
		_previousCloseLabel.textAlignment = UITextAlignmentLeft;
		_previousCloseLabel.text = NSLocalizedString(@"previousCloseLabel", @"LocalizedString");
		[self addSubview:_previousCloseLabel];
		
		_previousClose = [[UILabel alloc] initWithFrame:CGRectZero];
		_previousClose.font = regularDescription;
		_previousClose.textAlignment = UITextAlignmentRight;
		[self addSubview:_previousClose];
		
		_onVolumeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_onVolumeLabel.font = boldDescription;
		_onVolumeLabel.textAlignment = UITextAlignmentLeft;
		_onVolumeLabel.text = NSLocalizedString(@"onVolumeLabel", @"LocalizedString");
		[self addSubview:_onVolumeLabel];
		
		_onVolume = [[UILabel alloc] initWithFrame:CGRectZero];
		_onVolume.font = regularDescription;
		_onVolume.textAlignment = UITextAlignmentRight;
		[self addSubview:_onVolume];
		
		_onValueLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_onValueLabel.font = boldDescription;
		_onValueLabel.textAlignment = UITextAlignmentLeft;
		_onValueLabel.text = NSLocalizedString(@"onValueLabel", @"LocalizedString");
		[self addSubview:_onValueLabel];
		
		_onValue = [[UILabel alloc] initWithFrame:CGRectZero];
		_onValue.font = regularDescription;
		_onValue.textAlignment = UITextAlignmentRight;
		[self addSubview:_onValue];
	}
	return self;
}

- (void)layoutSubviews {
	UIFont *labelFont = [UIFont boldSystemFontOfSize:14.0];
	
	CGFloat globalY = 0.0f;
	
	CGSize descriptionSize = [_description.text sizeWithFont:labelFont];
	_description.frame = CGRectMake(0.0f, globalY, self.bounds.size.width, descriptionSize.height);
	globalY += descriptionSize.height; 
	
	CGSize isinLabelSize = [_isinLabel.text sizeWithFont:labelFont];
	_isinLabel.frame = CGRectMake(0.0f, globalY, isinLabelSize.width, isinLabelSize.height);
	
	CGSize isinSize = [_isin.text sizeWithFont:labelFont];
	_isin.frame = CGRectMake(0.0f + isinLabelSize.width, globalY, self.bounds.size.width - isinLabelSize.width, isinSize.height);
	globalY += isinSize.height;
	
	CGSize segmentLabelSize = [_segmentLabel.text sizeWithFont:labelFont];
	_segmentLabel.frame = CGRectMake(0.0f, globalY, segmentLabelSize.width, segmentLabelSize.height);
	
	CGSize segmentSize = [_segment.text sizeWithFont:labelFont];
	_segment.frame = CGRectMake(0.0f + segmentLabelSize.width, globalY, self.bounds.size.width - segmentLabelSize.width, segmentSize.height);
	globalY += segmentSize.height;
	
	CGSize currencyLabelSize = [_currencyLabel.text sizeWithFont:labelFont];
	_currencyLabel.frame = CGRectMake(0.0f, globalY, currencyLabelSize.width, currencyLabelSize.height);
	
	CGSize currencySize = [_currency.text sizeWithFont:labelFont];
	_currency.frame = CGRectMake(0.0f + currencyLabelSize.width, globalY, self.bounds.size.width - currencyLabelSize.width, currencySize.height);
	globalY += currencySize.height;
	
	CGSize countryLabelSize = [_countryLabel.text sizeWithFont:labelFont];
	_countryLabel.frame = CGRectMake(0.0f, globalY, countryLabelSize.width, countryLabelSize.height);
	
	CGSize countrySize = [_country.text sizeWithFont:labelFont];
	_country.frame = CGRectMake(0.0f + countryLabelSize.width, globalY, self.bounds.size.width - countryLabelSize.width, countrySize.height);
	globalY += countrySize.height;
	
	CGSize exchangeLabelSize = [_exchangeLabel.text sizeWithFont:labelFont];
	_exchangeLabel.frame = CGRectMake(0.0f, globalY, exchangeLabelSize.width, exchangeLabelSize.height);
	
	CGSize exchangeSize = [_exchange.text sizeWithFont:labelFont];
	_exchange.frame = CGRectMake(0.0f + exchangeLabelSize.width, globalY, self.bounds.size.width - exchangeLabelSize.width, exchangeSize.height);
	globalY += exchangeSize.height;
	
	CGSize statusLabelSize = [_statusLabel.text sizeWithFont:labelFont];
	_statusLabel.frame = CGRectMake(0.0f, globalY, statusLabelSize.width, statusLabelSize.height);
	
	CGSize statusSize = [_status.text sizeWithFont:labelFont];
	_status.frame = CGRectMake(0.0f + statusLabelSize.width, globalY, self.bounds.size.width - statusLabelSize.width, statusSize.height);
}

#pragma mark -
#pragma mark Symbol methods
- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", _symbol.feed.feedNumber, _symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] staticDataForFeedTicker:feedTicker];
	
	[_symbol addObserver:self forKeyPath:@"symbolDynamicData.lastTrade" options:NSKeyValueObservingOptionNew context:nil];
	
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

	_description.text = _symbol.companyName; 
	_isin.text = _symbol.isin;
	_segment.text = _symbol.symbolDynamicData.segment;
	_currency.text = _symbol.currency;
	_country.text = _symbol.country;
	_exchange.text = _symbol.feed.feedName;
	_status.text = _symbol.symbolDynamicData.tradingStatus;
	_dividend.text = [_symbol.symbolDynamicData.dividend stringValue];
	_dividendDate.text = [dateFormatter stringFromDate:_symbol.symbolDynamicData.dividendDate];
	_shares.text = _symbol.symbolDynamicData.outstandingShares;
	_marketCap.text = _symbol.symbolDynamicData.marketCapitalization;
	_previousClose.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.previousClose];
	_onVolume.text = _symbol.symbolDynamicData.onVolume;
	_onValue.text = _symbol.symbolDynamicData.onValue;
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
	[_symbol removeObserver:self forKeyPath:@"symbolDynamicData.lastTrade"];
	
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
