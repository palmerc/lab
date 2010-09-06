//
//  Chart.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Symbol;

@interface Chart :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * size;
@property (nonatomic, retain) NSData * data;
@property (nonatomic, retain) NSNumber * height;
@property (nonatomic, retain) NSNumber * width;
@property (nonatomic, retain) NSString * type;
@property (nonatomic, retain) Symbol * symbol;

@end



