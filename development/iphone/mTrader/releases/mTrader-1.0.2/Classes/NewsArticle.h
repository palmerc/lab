//
//  NewsArticle.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 20.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class NewsFeed;

@interface NewsArticle :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * time;
@property (nonatomic, retain) NSString * flag;
@property (nonatomic, retain) NSString * date;
@property (nonatomic, retain) NSString * articleNumber;
@property (nonatomic, retain) NSString * body;
@property (nonatomic, retain) NSString * headline;
@property (nonatomic, retain) NewsFeed * newsFeed;

@end



