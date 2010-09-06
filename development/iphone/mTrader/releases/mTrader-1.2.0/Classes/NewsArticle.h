//
//  NewsArticle.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class NewsFeed;
@class SymbolNewsRelationship;

@interface NewsArticle :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * time;
@property (nonatomic, retain) NSString * flag;
@property (nonatomic, retain) NSDate * date;
@property (nonatomic, retain) NSString * articleNumber;
@property (nonatomic, retain) NSString * body;
@property (nonatomic, retain) NSString * headline;
@property (nonatomic, retain) NSSet* symbols;
@property (nonatomic, retain) NewsFeed * newsFeed;

@end


@interface NewsArticle (CoreDataGeneratedAccessors)
- (void)addSymbolsObject:(SymbolNewsRelationship *)value;
- (void)removeSymbolsObject:(SymbolNewsRelationship *)value;
- (void)addSymbols:(NSSet *)value;
- (void)removeSymbols:(NSSet *)value;

@end

