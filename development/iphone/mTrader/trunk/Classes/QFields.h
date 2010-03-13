//
//  QFields.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


enum {
	FEEDTICKER = 0,
	TIMESTAMP,
	LAST_TRADE,
	CHANGE,
	CHANGE_PERCENT,
	CHANGE_ARROW,
	ASK_PRICE,
	ASK_VOLUME,
	BID_PRICE,
	BID_VOLUME,
	HIGH,
	LOW,
	OPEN,
	VOLUME,
	ORDERBOOK
} fields;

@interface QFields : NSObject {
@private
	BOOL qFieldsStateArray[ORDERBOOK + 1];
	
	BOOL timeStamp;
	BOOL lastTrade;
	BOOL change;
	BOOL changePercent;
	BOOL changeArrow;
	BOOL askPrice;
	BOOL askVolume;
	BOOL bidPrice;
	BOOL bidVolume;
	BOOL high;
	BOOL low;
	BOOL open;
	BOOL volume;
	BOOL orderBook;
}

@property (nonatomic) BOOL timeStamp;
@property (nonatomic) BOOL lastTrade;
@property (nonatomic) BOOL change;
@property (nonatomic) BOOL changePercent;
@property (nonatomic) BOOL changeArrow;
@property (nonatomic) BOOL askPrice;
@property (nonatomic) BOOL askVolume;
@property (nonatomic) BOOL bidPrice;
@property (nonatomic) BOOL bidVolume;
@property (nonatomic) BOOL high;
@property (nonatomic) BOOL low;
@property (nonatomic) BOOL open;
@property (nonatomic) BOOL volume;
@property (nonatomic) BOOL orderBook;

- (void)resetQFields;
- (NSString *)getCurrentQFieldsServerString;
- (NSString *)translateQFieldToMTraderServerCode:(int)number;
- (NSDictionary *)dictionaryFromQuote:(NSArray *)quote;

@end
