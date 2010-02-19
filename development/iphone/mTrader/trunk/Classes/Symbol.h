//
//  Symbol.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Chart;
@class Feed;
@class SymbolDynamicData;

@interface Symbol :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * tickerSymbol;
@property (nonatomic, retain) NSString * isin;
@property (nonatomic, retain) NSNumber * index;
@property (nonatomic, retain) NSString * type;
@property (nonatomic, retain) NSString * companyName;
@property (nonatomic, retain) NSString * country;
@property (nonatomic, retain) NSString * currency;
@property (nonatomic, retain) NSString * orderBook;
@property (nonatomic, retain) NSSet* orderBookEntries;
@property (nonatomic, retain) Chart * chart;
@property (nonatomic, retain) Feed * feed;
@property (nonatomic, retain) SymbolDynamicData * symbolDynamicData;

@end


@interface Symbol (CoreDataGeneratedAccessors)
- (void)addOrderBookEntriesObject:(NSManagedObject *)value;
- (void)removeOrderBookEntriesObject:(NSManagedObject *)value;
- (void)addOrderBookEntries:(NSSet *)value;
- (void)removeOrderBookEntries:(NSSet *)value;

@end

