//
//  Symbol.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "Symbol.h"


@implementation Symbol
@synthesize feedNumber = _feedNumber;
@synthesize tickerSymbol = _tickerSymbol;
@synthesize name = _name;
@synthesize isin = _isin;
@synthesize type = _type;
@synthesize orderbook = _orderbook;
@synthesize exchangeCode = _exchangeCode;
@synthesize status = _status;
@synthesize country = _country;
@synthesize currency = _currency;
@synthesize lastTrade = _lastTrade;
@synthesize lastTradeTime = _lastTradeTime;
@synthesize lastTradeChange = _lastTradeChange;
@synthesize lastTradePercentChange = _lastTradePercentChange;
@synthesize openChange = _openChange;
@synthesize openPercentChange = _openPercentChange;
@synthesize percentChange = _percentChange;
@synthesize bidPrice = _bidPrice;
@synthesize bidVolume = _bidVolume;
@synthesize bidSize = _bidSize; 
@synthesize askPrice = _askPrice;
@synthesize askVolume = _askVolume;
@synthesize askSize = _askSize;
@synthesize onVolume = _onVolume;
@synthesize onValue = _onValue;
@synthesize VWAP = _VWAP;
@synthesize averageVolume = _averageVolume;
@synthesize averageValue = _averageValue;
@synthesize marketCapitalization = _marketCapitalization;
@synthesize outstandingShares = _outstandingShares;
@synthesize change = _change;
@synthesize changeSinceLastUpdate = _changeSinceLastUpdate;
@synthesize low = _low;
@synthesize high = _high;
@synthesize open = _open;
@synthesize previousClose = _previousClose;
@synthesize volume = _volume;
@synthesize turnover = _turnover;
@synthesize buyLot = _buyLot;
@synthesize buyLotValue = _buyLotValue;
@synthesize chart = _chart;


- (id)init {
	self = [super init];
	if (self != nil) {
		_feedNumber = nil;
		_tickerSymbol = nil;
		_name = nil;
		_isin = nil;
		_type = nil;
		_orderbook = nil;
		_exchangeCode = nil;
		_status = nil;
		_country = nil;
		_currency = nil;
		_lastTrade = nil;
		_lastTradeTime = nil;
		_lastTradeChange = nil;
		_lastTradePercentChange = nil;
		_openChange = nil;
		_openPercentChange = nil;
		_percentChange = nil;
		_bidPrice = nil;
		_bidVolume = nil;
		_bidSize = nil; 
		_askPrice = nil;
		_askVolume = nil;
		_askSize = nil;
		_onVolume = nil;
		_onValue = nil;
		_VWAP = nil;
		_averageVolume = nil;
		_averageValue = nil;
		_marketCapitalization = nil;
		_outstandingShares = nil;
		_change = nil;
		_changeSinceLastUpdate = nil;
		_low = nil;
		_high = nil;
		_open = nil;
		_previousClose = nil;
		_volume = nil;
		_turnover = nil;
		_buyLot = nil;
		_buyLotValue = nil;
		_chart = nil;
	}
	
	return self;
}

- (NSString *)feedTicker {
	return [NSString stringWithFormat:@"%@/%@", self.feedNumber, self.tickerSymbol];
}

- (void)setFeedTicker:(NSString *)feedTicker {
	NSArray *components = [feedTicker componentsSeparatedByString:@"/"];
	assert([components count] == 2);
	
	// Format of feedTicker string is feedNumber/tickerSymbol
	NSString *feedNumber = [components objectAtIndex:0];
	NSString *tickerSymbol = [components objectAtIndex:1];
	
	self.feedNumber = feedNumber;
	self.tickerSymbol = tickerSymbol;
}

//-(void)setLastTrade:(NSString *)trade {
//	[trade retain];	
//	float lt = [self.lastTrade floatValue];
//	float t = [trade floatValue];
//	float c = t - lt;
//	//NSLog(@"changeSinceLastTrade: %f", c);
//	self.changeSinceLastUpdate = [NSString stringWithFormat:@"%f", c];
//	[lastTrade release];
//	lastTrade = nil;
//	lastTrade = trade;
//	
//}	

- (void)dealloc {
	[self.feedNumber release];
	[self.tickerSymbol release];
	[self.name release];
	[self.isin release];
	[self.type release];
	[self.orderbook release];
	[self.exchangeCode release];
	[self.status release];
	[self.country release];
	[self.currency release];
	[self.lastTrade release];
	[self.lastTradeTime release];
	[self.lastTradeChange release];
	[self.lastTradePercentChange release];
	[self.openChange release];
	[self.openPercentChange release];
	[self.percentChange release];
	[self.bidPrice release];
	[self.bidVolume release];
	[self.bidSize release]; 
	[self.askPrice release];
	[self.askVolume release];
	[self.askSize release];
	[self.onVolume release];
	[self.onValue release];
	[self.VWAP release];
	[self.averageVolume release];
	[self.averageValue release];
	[self.marketCapitalization release];
	[self.outstandingShares release];
	[self.change release];
	[self.changeSinceLastUpdate release];
	[self.low release];
	[self.high release];
	[self.open release];
	[self.previousClose release];
	[self.volume release];
	[self.turnover release];
	[self.buyLot release];
	[self.buyLotValue release];
	[self.chart release];
	
	[super dealloc];
}

- (NSString *)description {
	return [NSString stringWithFormat:@"(Symbol: %@ %@, ISIN: %@, Type: %@, Orderbook: %@, ExchangeCode: %@)", self.feedTicker, self.name, self.isin, self.type, self.orderbook, self.exchangeCode];
}

- (BOOL)isEqual:(id)anObject {
	if ([anObject isKindOfClass:[Symbol class]]) {
		Symbol *symbol = (Symbol *)anObject;
		if ([[self feedTicker] isEqual:[symbol feedTicker]]) {
			return YES;
		}
	}
	return NO;
}

@end
