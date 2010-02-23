// 
//  Symbol.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Symbol.h"

#import "Chart.h"
#import "Feed.h"
#import "OrderBookEntry.h"
#import "SymbolDynamicData.h"

@implementation Symbol 

@dynamic orderBook;
@dynamic tickerSymbol;
@dynamic isin;
@dynamic index;
@dynamic type;
@dynamic companyName;
@dynamic country;
@dynamic currency;
@dynamic chart;
@dynamic feed;
@dynamic symbolDynamicData;
@dynamic orderBookEntries;

@end
