//
//  Trade.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Symbol;

@interface Trade :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * volume;
@property (nonatomic, retain) NSString * time;
@property (nonatomic, retain) NSString * seller;
@property (nonatomic, retain) NSString * price;
@property (nonatomic, retain) NSString * buyer;
@property (nonatomic, retain) NSString * type;
@property (nonatomic, retain) NSNumber * index;
@property (nonatomic, retain) Symbol * symbol;

@end



