//
//  TradesLiveInfoView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesLiveInfoView.h"

#import "mTraderCommunicator.h"
#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Chart.h"


@implementation TradesLiveInfoView
@synthesize symbol = _symbol;
@synthesize openLabel = _openLabel; 
@synthesize open = _open;
@synthesize highLabel = _highLabel;
@synthesize high = _high;
@synthesize lowLabel = _lowLabel;
@synthesize low = _low;
@synthesize vwapLabel = _vwapLabel;
@synthesize vwap = _vwap;
@synthesize volumeLabel = _volumeLabel;
@synthesize volume = _volume;
@synthesize tradesLabel = _tradesLabel;
@synthesize trades = _trades;
@synthesize turnoverLabel = _turnoverLabel;
@synthesize turnover = _turnover;
@synthesize bLotLabel = _bLotLabel;
@synthesize bLot = _bLot;
@synthesize bLotValLabel = _bLotValLabel;
@synthesize bLotVal = _bLotVal;
@synthesize avgVolLabel = _avgVolLabel;
@synthesize avgVol = _avgVol;
@synthesize avgValLabel = _avgValLabel;
@synthesize avgVal = _avgVal;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self != nil) {
		UIFont *labelFont = [UIFont boldSystemFontOfSize:14.0];
		UIFont *dataFont = [UIFont systemFontOfSize:14.0];
		
		_openLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_openLabel.font = labelFont;
		_openLabel.textAlignment = UITextAlignmentLeft;
		_openLabel.text = NSLocalizedString(@"open", @"LocalizedString");
		[self addSubview:_openLabel];
		
		_open = [[UILabel alloc] initWithFrame:CGRectZero];
		_open.font = dataFont;
		_open.adjustsFontSizeToFitWidth = YES;
		_open.textAlignment = UITextAlignmentRight;
		[self addSubview:_open];
		
		_highLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_highLabel.font = labelFont;
		_highLabel.textAlignment = UITextAlignmentLeft;
		_highLabel.text = NSLocalizedString(@"high", @"LocalizedString");
		[self addSubview:_highLabel];
		
		_high = [[UILabel alloc] initWithFrame:CGRectZero];
		_high.font = dataFont;
		_high.adjustsFontSizeToFitWidth = YES;
		_high.textAlignment = UITextAlignmentRight;
		[self addSubview:_high];
		
		_lowLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_lowLabel.font = labelFont;
		_lowLabel.textAlignment = UITextAlignmentLeft;
		_lowLabel.text = NSLocalizedString(@"low", @"LocalizedString");
		[self addSubview:_lowLabel];
		
		_low = [[UILabel alloc] initWithFrame:CGRectZero];
		_low.font = dataFont;
		_low.adjustsFontSizeToFitWidth = YES;
		_low.textAlignment = UITextAlignmentRight;
		[self addSubview:_low];
		
		_vwapLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_vwapLabel.font = labelFont;
		_vwapLabel.textAlignment = UITextAlignmentLeft;
		_vwapLabel.text = NSLocalizedString(@"vwap", @"LocalizedString");
		[self addSubview:_vwapLabel];
		
		_vwap = [[UILabel alloc] initWithFrame:CGRectZero];
		_vwap.font = dataFont;
		_vwap.adjustsFontSizeToFitWidth = YES;
		_vwap.textAlignment = UITextAlignmentRight;
		[self addSubview:_vwap];
		
		_volumeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_volumeLabel.font = labelFont;
		_volumeLabel.textAlignment = UITextAlignmentLeft;
		_volumeLabel.text = NSLocalizedString(@"volume", @"LocalizedString");
		[self addSubview:_volumeLabel];
		
		_volume = [[UILabel alloc] initWithFrame:CGRectZero];
		_volume.font = dataFont;
		_volume.adjustsFontSizeToFitWidth = YES;
		_volume.textAlignment = UITextAlignmentRight;
		[self addSubview:_volume];
		
		_tradesLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_tradesLabel.font = labelFont;
		_tradesLabel.textAlignment = UITextAlignmentLeft;
		_tradesLabel.text = NSLocalizedString(@"trades", @"LocalizedString");
		[self addSubview:_tradesLabel];
		
		_trades = [[UILabel alloc] initWithFrame:CGRectZero];
		_trades.font = dataFont;
		_trades.adjustsFontSizeToFitWidth = YES;
		_trades.textAlignment = UITextAlignmentRight;
		[self addSubview:_trades];
		
		_turnoverLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_turnoverLabel.font = labelFont;
		_turnoverLabel.textAlignment = UITextAlignmentLeft;
		_turnoverLabel.text = NSLocalizedString(@"turnover", @"LocalizedString");
		[self addSubview:_turnoverLabel];
		
		_turnover = [[UILabel alloc] initWithFrame:CGRectZero];
		_turnover.font = dataFont;
		_turnover.adjustsFontSizeToFitWidth = YES;
		_turnover.textAlignment = UITextAlignmentRight;
		[self addSubview:_turnover];
		
		_bLotLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_bLotLabel.font = labelFont;
		_bLotLabel.textAlignment = UITextAlignmentLeft;
		_bLotLabel.text = NSLocalizedString(@"bLot", @"LocalizedString");
		[self addSubview:_bLotLabel];
		
		_bLot = [[UILabel alloc] initWithFrame:CGRectZero];
		_bLot.font = dataFont;
		_bLot.adjustsFontSizeToFitWidth = YES;
		_bLot.textAlignment = UITextAlignmentRight;
		[self addSubview:_bLot];
		
		_bLotValLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_bLotValLabel.font = labelFont;
		_bLotValLabel.textAlignment = UITextAlignmentLeft;
		_bLotValLabel.text = NSLocalizedString(@"bLotVal", @"LocalizedString");
		[self addSubview:_bLotValLabel];
		
		_bLotVal = [[UILabel alloc] initWithFrame:CGRectZero];
		_bLotVal.font = dataFont;
		_bLotVal.adjustsFontSizeToFitWidth = YES;
		_bLotVal.textAlignment = UITextAlignmentRight;
		[self addSubview:_bLotVal];
		
		_avgVolLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_avgVolLabel.font = labelFont;
		_avgVolLabel.textAlignment = UITextAlignmentLeft;
		_avgVolLabel.text = NSLocalizedString(@"avgVol", @"LocalizedString");
		[self addSubview:_avgVolLabel];
		
		_avgVol = [[UILabel alloc] initWithFrame:CGRectZero];
		_avgVol.font = dataFont;
		_avgVol.adjustsFontSizeToFitWidth = YES;
		_avgVol.textAlignment = UITextAlignmentRight;
		[self addSubview:_avgVol];
		
		_avgValLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_avgValLabel.font = labelFont;
		_avgValLabel.textAlignment = UITextAlignmentLeft;
		_avgValLabel.text = NSLocalizedString(@"avgVal", @"LocalizedString");
		[self addSubview:_avgValLabel];
		
		_avgVal = [[UILabel alloc] initWithFrame:CGRectZero];
		_avgVal.font = dataFont;
		_avgVal.adjustsFontSizeToFitWidth = YES;
		_avgVal.textAlignment = UITextAlignmentRight;
		[self addSubview:_avgVal];
	}
	return self;
}

- (void)layoutSubviews {
		CGFloat leftPadding = 0.0f;
		CGFloat width = self.bounds.size.width;
		
		UIFont *labelFont = [UIFont boldSystemFontOfSize:14.0];
		
		CGFloat globalY = leftPadding;
		CGSize openLabelSize = [_openLabel.text sizeWithFont:labelFont];
		_openLabel.frame = CGRectMake(leftPadding, globalY, openLabelSize.width, openLabelSize.height);
		_open.frame = CGRectMake(leftPadding + openLabelSize.width, globalY, width - openLabelSize.width, openLabelSize.height);
		globalY += openLabelSize.height;
		
		CGSize highLabelSize = [_highLabel.text sizeWithFont:labelFont];
		_highLabel.frame = CGRectMake(leftPadding, globalY, highLabelSize.width, highLabelSize.height);
		_high.frame = CGRectMake(leftPadding + highLabelSize.width, globalY, width - highLabelSize.width, highLabelSize.height);
		globalY += highLabelSize.height;
		
		CGSize lowLabelSize = [_lowLabel.text sizeWithFont:labelFont];
		_lowLabel.frame = CGRectMake(leftPadding, globalY, lowLabelSize.width, lowLabelSize.height);
		_low.frame = CGRectMake(leftPadding + lowLabelSize.width, globalY, width - lowLabelSize.width, lowLabelSize.height);
		globalY += lowLabelSize.height;
		
		CGSize vwapLabelSize = [_vwapLabel.text sizeWithFont:labelFont];
		_vwapLabel.frame = CGRectMake(leftPadding, globalY, vwapLabelSize.width, vwapLabelSize.height);
		_vwap.frame = CGRectMake(leftPadding + vwapLabelSize.width, globalY, width - vwapLabelSize.width, vwapLabelSize.height);
		globalY += vwapLabelSize.height;
		
		CGSize volumeLabelSize = [_volumeLabel.text sizeWithFont:labelFont];
		_volumeLabel.frame = CGRectMake(leftPadding, globalY, volumeLabelSize.width, volumeLabelSize.height);
		_volume.frame = CGRectMake(leftPadding + volumeLabelSize.width, globalY, width - volumeLabelSize.width, volumeLabelSize.height);
		globalY += volumeLabelSize.height;
		
		CGSize tradesLabelSize = [_tradesLabel.text sizeWithFont:labelFont];
		_tradesLabel.frame = CGRectMake(leftPadding, globalY, tradesLabelSize.width, tradesLabelSize.height);
		_trades.frame = CGRectMake(leftPadding + tradesLabelSize.width, globalY, width - tradesLabelSize.width, tradesLabelSize.height);
		globalY += tradesLabelSize.height;
		
		CGSize turnoverLabelSize = [_turnoverLabel.text sizeWithFont:labelFont];
		_turnoverLabel.frame = CGRectMake(leftPadding, globalY, turnoverLabelSize.width, turnoverLabelSize.height);
		_turnover.frame = CGRectMake(leftPadding + turnoverLabelSize.width, globalY, width - turnoverLabelSize.width, turnoverLabelSize.height);
		globalY += turnoverLabelSize.height;
		
		CGSize bLotLabelSize = [_bLotLabel.text sizeWithFont:labelFont];
		_bLotLabel.frame = CGRectMake(leftPadding, globalY, bLotLabelSize.width, bLotLabelSize.height);
		_bLot.frame = CGRectMake(leftPadding + bLotLabelSize.width, globalY, width - bLotLabelSize.width, bLotLabelSize.height);
		globalY += bLotLabelSize.height;
		
		CGSize bLotValLabelSize = [_bLotValLabel.text sizeWithFont:labelFont];
		_bLotValLabel.frame = CGRectMake(leftPadding, globalY, bLotValLabelSize.width, bLotValLabelSize.height);
		_bLotVal.frame = CGRectMake(leftPadding + bLotValLabelSize.width, globalY, width - bLotValLabelSize.width, bLotValLabelSize.height);
		globalY += bLotValLabelSize.height;
		
		CGSize avgVolLabelSize = [_avgVolLabel.text sizeWithFont:labelFont];
		_avgVolLabel.frame = CGRectMake(leftPadding, globalY, avgVolLabelSize.width, avgVolLabelSize.height);
		_avgVol.frame = CGRectMake(leftPadding + avgVolLabelSize.width, globalY, width - avgVolLabelSize.width, avgVolLabelSize.height);
		globalY += avgVolLabelSize.height;
		
		CGSize avgValLabelSize = [_avgValLabel.text sizeWithFont:labelFont];
		_avgValLabel.frame = CGRectMake(leftPadding, globalY, avgValLabelSize.width, avgValLabelSize.height);
		_avgVal.frame = CGRectMake(leftPadding + avgValLabelSize.width, globalY, width - avgValLabelSize.width, avgValLabelSize.height);	

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
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	}
	
	_open.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.open];
	_high.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.high];
	_low.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.low];
	_vwap.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.VWAP];
	_volume.text = _symbol.symbolDynamicData.volume;
	_trades.text = _symbol.symbolDynamicData.trades;
	_turnover.text = _symbol.symbolDynamicData.turnover;
	_bLot.text = _symbol.symbolDynamicData.buyLot;
	_bLotVal.text = _symbol.symbolDynamicData.buyLotValue;
	_avgVal.text = _symbol.symbolDynamicData.averageValue;
	_avgVol.text = _symbol.symbolDynamicData.averageVolume;
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
	[_openLabel release]; 
	[_open release];
	[_highLabel release];
	[_high release];
	[_lowLabel release];
	[_low release];
	[_vwapLabel release];
	[_vwap release];
	[_volumeLabel release];
	[_volume release];
	[_tradesLabel release];
	[_trades release];
	[_turnoverLabel release];
	[_turnover release];
	[_bLotLabel release];
	[_bLot release];
	[_bLotValLabel release];
	[_bLotVal release];
	[_avgVolLabel release];
	[_avgVol release];
	[_avgValLabel release];
	[_avgVal release];
	
	
	[super dealloc];
}

@end
