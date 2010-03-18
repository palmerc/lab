// 
//  Symbol.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Symbol.h"

#import "BidAsk.h"
#import "Chart.h"
#import "Feed.h"
#import "SymbolDynamicData.h"

@implementation Symbol 

@dynamic index;
@dynamic tickerSymbol;
@dynamic isin;
@dynamic type;
@dynamic companyName;
@dynamic country;
@dynamic currency;
@dynamic orderBook;
@dynamic chart;
@dynamic feed;
@dynamic bidsAsks;
@dynamic symbolDynamicData;

@end
