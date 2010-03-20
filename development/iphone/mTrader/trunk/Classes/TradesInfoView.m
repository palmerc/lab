//
//  TradesInfoView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesInfoView.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Chart.h"


@implementation TradesInfoView
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
		_openLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.openLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.openLabel.textAlignment = UITextAlignmentLeft;
		self.openLabel.text = NSLocalizedString(@"open", @"LocalizedString");
		[self addSubview:self.openLabel];
		
		_open = [[UILabel alloc] initWithFrame:CGRectZero];
		self.open.font = [UIFont systemFontOfSize:14.0];
		self.open.adjustsFontSizeToFitWidth = YES;
		self.open.textAlignment = UITextAlignmentRight;
		[self addSubview:self.open];
		
		_highLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.highLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.highLabel.textAlignment = UITextAlignmentLeft;
		self.highLabel.text = NSLocalizedString(@"high", @"LocalizedString");
		[self addSubview:self.highLabel];
		
		_high = [[UILabel alloc] initWithFrame:CGRectZero];
		self.high.font = [UIFont systemFontOfSize:14.0];
		self.high.adjustsFontSizeToFitWidth = YES;
		self.high.textAlignment = UITextAlignmentRight;
		[self addSubview:self.high];
		
		_lowLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.lowLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.lowLabel.textAlignment = UITextAlignmentLeft;
		self.lowLabel.text = NSLocalizedString(@"low", @"LocalizedString");
		[self addSubview:self.lowLabel];
		
		_low = [[UILabel alloc] initWithFrame:CGRectZero];
		self.low.font = [UIFont systemFontOfSize:14.0];
		self.low.adjustsFontSizeToFitWidth = YES;
		self.low.textAlignment = UITextAlignmentRight;
		[self addSubview:self.low];
		
		_vwapLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.vwapLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.vwapLabel.textAlignment = UITextAlignmentLeft;
		self.vwapLabel.text = NSLocalizedString(@"vwap", @"LocalizedString");
		[self addSubview:self.vwapLabel];
		
		_vwap = [[UILabel alloc] initWithFrame:CGRectZero];
		self.vwap.font = [UIFont systemFontOfSize:14.0];
		self.vwap.adjustsFontSizeToFitWidth = YES;
		self.vwap.textAlignment = UITextAlignmentRight;
		[self addSubview:self.vwap];
		
		_volumeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.volumeLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.volumeLabel.textAlignment = UITextAlignmentLeft;
		self.volumeLabel.text = NSLocalizedString(@"volume", @"LocalizedString");
		[self addSubview:self.volumeLabel];
		
		_volume = [[UILabel alloc] initWithFrame:CGRectZero];
		self.volume.font = [UIFont systemFontOfSize:14.0];
		self.volume.adjustsFontSizeToFitWidth = YES;
		self.volume.textAlignment = UITextAlignmentRight;
		[self addSubview:self.volume];
		
		_tradesLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.tradesLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.tradesLabel.textAlignment = UITextAlignmentLeft;
		self.tradesLabel.text = NSLocalizedString(@"trades", @"LocalizedString");
		[self addSubview:self.tradesLabel];
		
		_trades = [[UILabel alloc] initWithFrame:CGRectZero];
		self.trades.font = [UIFont systemFontOfSize:14.0];
		self.trades.adjustsFontSizeToFitWidth = YES;
		self.trades.textAlignment = UITextAlignmentRight;
		[self addSubview:self.trades];
		
		_turnoverLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.turnoverLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.turnoverLabel.textAlignment = UITextAlignmentLeft;
		self.turnoverLabel.text = NSLocalizedString(@"turnover", @"LocalizedString");
		[self addSubview:self.turnoverLabel];
		
		_turnover = [[UILabel alloc] initWithFrame:CGRectZero];
		self.turnover.font = [UIFont systemFontOfSize:14.0];
		self.turnover.adjustsFontSizeToFitWidth = YES;
		self.turnover.textAlignment = UITextAlignmentRight;
		[self addSubview:self.turnover];
		
		_bLotLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.bLotLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.bLotLabel.textAlignment = UITextAlignmentLeft;
		self.bLotLabel.text = NSLocalizedString(@"bLot", @"LocalizedString");
		[self addSubview:self.bLotLabel];
		
		_bLot = [[UILabel alloc] initWithFrame:CGRectZero];
		self.bLot.font = [UIFont systemFontOfSize:14.0];
		self.bLot.adjustsFontSizeToFitWidth = YES;
		self.bLot.textAlignment = UITextAlignmentRight;
		[self addSubview:self.bLot];
		
		_bLotValLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.bLotValLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.bLotValLabel.textAlignment = UITextAlignmentLeft;
		self.bLotValLabel.text = NSLocalizedString(@"bLotVal", @"LocalizedString");
		[self addSubview:self.bLotValLabel];
		
		_bLotVal = [[UILabel alloc] initWithFrame:CGRectZero];
		self.bLotVal.font = [UIFont systemFontOfSize:14.0];
		self.bLotVal.adjustsFontSizeToFitWidth = YES;
		self.bLotVal.textAlignment = UITextAlignmentRight;
		[self addSubview:self.bLotVal];
		
		_avgVolLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.avgVolLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.avgVolLabel.textAlignment = UITextAlignmentLeft;
		self.avgVolLabel.text = NSLocalizedString(@"avgVol", @"LocalizedString");
		[self addSubview:self.avgVolLabel];
		
		_avgVol = [[UILabel alloc] initWithFrame:CGRectZero];
		self.avgVol.font = [UIFont systemFontOfSize:14.0];
		self.avgVol.adjustsFontSizeToFitWidth = YES;
		self.avgVol.textAlignment = UITextAlignmentRight;
		[self addSubview:self.avgVol];
		
		_avgValLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.avgValLabel.font = [UIFont boldSystemFontOfSize:14.0];
		self.avgValLabel.textAlignment = UITextAlignmentLeft;
		self.avgValLabel.text = NSLocalizedString(@"avgVal", @"LocalizedString");
		[self addSubview:self.avgValLabel];
		
		_avgVal = [[UILabel alloc] initWithFrame:CGRectZero];
		self.avgVal.font = [UIFont systemFontOfSize:14.0];
		self.avgVal.adjustsFontSizeToFitWidth = YES;
		self.avgVal.textAlignment = UITextAlignmentRight;
		[self addSubview:self.avgVal];
	}
	return self;
}

#pragma mark -
#pragma mark UIView drawing
- (void)drawRect:(CGRect)rect {
	super.padding = 6.0f;
	super.cornerRadius = 10.0f;
	super.strokeWidth = 0.75f;
	[super drawRect:rect];
	
	CGFloat leftPadding = ceilf(self.padding + super.strokeWidth + kBlur);
	CGFloat maxWidth = rect.size.width - leftPadding * 2.0f;
		
	UIFont *labelFont = [UIFont boldSystemFontOfSize:14.0];
	
	CGFloat globalY = leftPadding;
	CGSize openLabelSize = [self.openLabel.text sizeWithFont:labelFont];
	self.openLabel.frame = CGRectMake(leftPadding, globalY, openLabelSize.width, openLabelSize.height);
	self.open.frame = CGRectMake(leftPadding + openLabelSize.width, globalY, maxWidth - openLabelSize.width, openLabelSize.height);
	globalY += openLabelSize.height;
	
	CGSize highLabelSize = [self.highLabel.text sizeWithFont:labelFont];
	self.highLabel.frame = CGRectMake(leftPadding, globalY, highLabelSize.width, highLabelSize.height);
	self.high.frame = CGRectMake(leftPadding + highLabelSize.width, globalY, maxWidth - highLabelSize.width, highLabelSize.height);
	globalY += highLabelSize.height;
	
	CGSize lowLabelSize = [self.lowLabel.text sizeWithFont:labelFont];
	self.lowLabel.frame = CGRectMake(leftPadding, globalY, lowLabelSize.width, lowLabelSize.height);
	self.low.frame = CGRectMake(leftPadding + lowLabelSize.width, globalY, maxWidth - lowLabelSize.width, lowLabelSize.height);
	globalY += lowLabelSize.height;
	
	CGSize vwapLabelSize = [self.vwapLabel.text sizeWithFont:labelFont];
	self.vwapLabel.frame = CGRectMake(leftPadding, globalY, vwapLabelSize.width, vwapLabelSize.height);
	self.vwap.frame = CGRectMake(leftPadding + vwapLabelSize.width, globalY, maxWidth - vwapLabelSize.width, vwapLabelSize.height);
	globalY += vwapLabelSize.height;
	
	CGSize volumeLabelSize = [self.volumeLabel.text sizeWithFont:labelFont];
	self.volumeLabel.frame = CGRectMake(leftPadding, globalY, volumeLabelSize.width, volumeLabelSize.height);
	self.volume.frame = CGRectMake(leftPadding + volumeLabelSize.width, globalY, maxWidth - volumeLabelSize.width, volumeLabelSize.height);
	globalY += volumeLabelSize.height;
	
	CGSize tradesLabelSize = [self.tradesLabel.text sizeWithFont:labelFont];
	self.tradesLabel.frame = CGRectMake(leftPadding, globalY, tradesLabelSize.width, tradesLabelSize.height);
	self.trades.frame = CGRectMake(leftPadding + tradesLabelSize.width, globalY, maxWidth - tradesLabelSize.width, tradesLabelSize.height);
	globalY += tradesLabelSize.height;
	
	CGSize turnoverLabelSize = [self.turnoverLabel.text sizeWithFont:labelFont];
	self.turnoverLabel.frame = CGRectMake(leftPadding, globalY, turnoverLabelSize.width, turnoverLabelSize.height);
	self.turnover.frame = CGRectMake(leftPadding + turnoverLabelSize.width, globalY, maxWidth - turnoverLabelSize.width, turnoverLabelSize.height);
	globalY += turnoverLabelSize.height;
	
	CGSize bLotLabelSize = [self.bLotLabel.text sizeWithFont:labelFont];
	self.bLotLabel.frame = CGRectMake(leftPadding, globalY, bLotLabelSize.width, bLotLabelSize.height);
	self.bLot.frame = CGRectMake(leftPadding + bLotLabelSize.width, globalY, maxWidth - bLotLabelSize.width, bLotLabelSize.height);
	globalY += bLotLabelSize.height;
	
	CGSize bLotValLabelSize = [self.bLotValLabel.text sizeWithFont:labelFont];
	self.bLotValLabel.frame = CGRectMake(leftPadding, globalY, bLotValLabelSize.width, bLotValLabelSize.height);
	self.bLotVal.frame = CGRectMake(leftPadding + bLotValLabelSize.width, globalY, maxWidth - bLotValLabelSize.width, bLotValLabelSize.height);
	globalY += bLotValLabelSize.height;

	CGSize avgVolLabelSize = [self.avgVolLabel.text sizeWithFont:labelFont];
	self.avgVolLabel.frame = CGRectMake(leftPadding, globalY, avgVolLabelSize.width, avgVolLabelSize.height);
	self.avgVol.frame = CGRectMake(leftPadding + avgVolLabelSize.width, globalY, maxWidth - avgVolLabelSize.width, avgVolLabelSize.height);
	globalY += avgVolLabelSize.height;
	
	CGSize avgValLabelSize = [self.avgValLabel.text sizeWithFont:labelFont];
	self.avgValLabel.frame = CGRectMake(leftPadding, globalY, avgValLabelSize.width, avgValLabelSize.height);
	self.avgVal.frame = CGRectMake(leftPadding + avgValLabelSize.width, globalY, maxWidth - avgValLabelSize.width, avgValLabelSize.height);	
}

#pragma mark -
#pragma mark Symbol methods
- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	
	[self.symbol addObserver:self forKeyPath:@"symbolDynamicData.lastTrade" options:NSKeyValueObservingOptionNew context:nil];
	
	[self updateSymbol];
	[self setNeedsDisplay];
}

- (void)updateSymbol {
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	}
	
	self.open.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.open];
	self.high.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.high];
	self.low.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.low];
	self.vwap.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.VWAP];
	self.volume.text = self.symbol.symbolDynamicData.volume;
	self.trades.text = self.symbol.symbolDynamicData.trades;
	self.turnover.text = self.symbol.symbolDynamicData.turnover;
	self.bLot.text = self.symbol.symbolDynamicData.buyLot;
	self.bLotVal.text = self.symbol.symbolDynamicData.buyLotValue;
	self.avgVal.text = self.symbol.symbolDynamicData.averageValue;
	self.avgVol.text = self.symbol.symbolDynamicData.averageVolume;
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
