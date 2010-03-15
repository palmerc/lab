//
//  NewsFeed.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 26.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import <CoreData/CoreData.h>


@interface NewsFeed :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * feedNumber;
@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSString * mCode;
@property (nonatomic, retain) NSString * type;

@end



