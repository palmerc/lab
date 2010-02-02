//
//  Symbol.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Feed;

@interface Symbol :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * currency;
@property (nonatomic, retain) NSString * country;
@property (nonatomic, retain) NSString * tickerSymbol;
@property (nonatomic, retain) NSString * companyName;
@property (nonatomic, retain) NSString * isin;
@property (nonatomic, retain) NSString * orderBook;
@property (nonatomic, retain) NSString * type;
@property (nonatomic, retain) NSManagedObject * symbolDynamicData;
@property (nonatomic, retain) Feed * feed;
@property (nonatomic, retain) NSManagedObject * chart;

@end



