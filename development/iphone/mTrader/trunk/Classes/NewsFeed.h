//
//  NewsFeed.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 20.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class NewsArticle;

@interface NewsFeed :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * feedNumber;
@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSString * mCode;
@property (nonatomic, retain) NSString * type;
@property (nonatomic, retain) NSSet* newsArticles;

@end


@interface NewsFeed (CoreDataGeneratedAccessors)
- (void)addNewsArticlesObject:(NewsArticle *)value;
- (void)removeNewsArticlesObject:(NewsArticle *)value;
- (void)addNewsArticles:(NSSet *)value;
- (void)removeNewsArticles:(NSSet *)value;

@end

