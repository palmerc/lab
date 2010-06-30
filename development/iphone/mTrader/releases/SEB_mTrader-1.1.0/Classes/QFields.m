//
//  QFields.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "QFields.h"


@implementation QFields
@synthesize timeStamp, lastTrade, change, changePercent, changeArrow, askPrice, askVolume, bidPrice, bidVolume, high, low, open, volume, orderBook;

#pragma mark -
#pragma mark Initialization
- (id)init {
	self = [super init];
	if (self != nil) {		
		[self resetQFields];
	}
	return self;
}

- (void)setTimeStamp:(BOOL)state {
	timeStamp = qFieldsStateArray[TIMESTAMP] = state;
}

- (void)setLastTrade:(BOOL)state {
	lastTrade = qFieldsStateArray[LAST_TRADE] = state;
}

- (void)setChange:(BOOL)state {
	change = qFieldsStateArray[CHANGE] = state;
}

- (void)setChangePercent:(BOOL)state {
	changePercent = qFieldsStateArray[CHANGE_PERCENT] = state;
}

- (void)setChangeArrow:(BOOL)state {
	changeArrow = qFieldsStateArray[CHANGE_ARROW] = state;
}

- (void)setAskPrice:(BOOL)state {
	askPrice = qFieldsStateArray[ASK_PRICE] = state;
}

- (void)setAskVolume:(BOOL)state {
	askVolume = qFieldsStateArray[ASK_VOLUME] = state;
}

- (void)setBidPrice:(BOOL)state {
	bidPrice = qFieldsStateArray[BID_PRICE] = state;
}

- (void)setBidVolume:(BOOL)state {
	bidVolume = qFieldsStateArray[BID_VOLUME] = state;
}

- (void)setHigh:(BOOL)state {
	high = qFieldsStateArray[HIGH] = state;
}

- (void)setLow:(BOOL)state {
	low = qFieldsStateArray[LOW] = state;
}

- (void)setOpen:(BOOL)state {
	open = qFieldsStateArray[OPEN] = state;
}

- (void)setVolume:(BOOL)state {
	volume = qFieldsStateArray[VOLUME] = state;
}

- (void)setOrderBook:(BOOL)state {
	orderBook = qFieldsStateArray[ORDERBOOK] = state;
}


#pragma mark -
#pragma mark qFields management methods

- (void)resetQFields {
	self.timeStamp = NO;
	self.lastTrade = NO;
	self.change = NO;
	self.changePercent = NO;
	self.changeArrow = NO;
	self.askPrice = NO;
	self.askVolume = NO;
	self.bidPrice = NO;
	self.bidVolume = NO;
	self.high = NO;
	self.low = NO;
	self.open = NO;
	self.volume = NO;
	self.orderBook = NO;
}

- (NSString *)getCurrentQFieldsServerString {
	NSMutableArray *qFields = [[NSMutableArray alloc] init];
	for (int i = 0; i < ( sizeof(qFieldsStateArray)/sizeof(BOOL) ); i++) {
		if (qFieldsStateArray[i] == YES) {
			[qFields addObject:[self translateQFieldToMTraderServerCode:i]];
		}
	}
	
	NSString *qFieldsInServerStringFormat = [qFields componentsJoinedByString:@";"];
	[qFields release];
	
	return qFieldsInServerStringFormat;
}

- (NSString *)translateQFieldToMTraderServerCode:(int)number {
	switch (number) {
		case TIMESTAMP:
			return @"t";
			break;
		case LAST_TRADE:
			return @"l";
			break;
		case CHANGE:
			return @"c";
			break;
		case CHANGE_PERCENT:
			return @"cp";
			break;
		case CHANGE_ARROW:
			return @"la";
			break;
		case ASK_PRICE:
			return @"a";
			break;
		case ASK_VOLUME:
			return @"av";
			break;
		case BID_PRICE:
			return @"b";
			break;
		case BID_VOLUME:
			return @"bv";
			break;
		case HIGH:
			return @"h";
			break;
		case LOW:
			return @"lo";
			break;
		case OPEN:
			return @"o";
			break;
		case VOLUME:
			return @"v";
			break;
		case ORDERBOOK:
			return @"d";
			break;
		default:
			return nil;
			break;
	}
}

- (NSDictionary *)dictionaryFromQuote:(NSArray *)quote {
	NSMutableDictionary *dictionary = [[[NSMutableDictionary alloc] init] autorelease];
	
	NSString *feedTicker = [quote objectAtIndex:0];
	[dictionary setValue:feedTicker forKey:@"feedTicker"];
	
	NSMutableArray *revisedQuote = [NSMutableArray arrayWithArray:quote];
	[revisedQuote removeObjectAtIndex:0];
	quote = revisedQuote;
	
	int i = 0;
	for (NSString *value in quote) {
		while (qFieldsStateArray[i] == NO) {
			i++;
		} 
		NSString *key = [NSString stringWithFormat:@"%d", i];
		[dictionary setValue:value forKey:key];
		i++;
	}
	return dictionary;
}
				
#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[super dealloc];
}
@end
