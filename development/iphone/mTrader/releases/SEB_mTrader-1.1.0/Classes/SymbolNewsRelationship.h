//
//  SymbolNewsRelationship.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 26.06.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class NewsArticle;
@class Symbol;

@interface SymbolNewsRelationship :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * feedTicker;
@property (nonatomic, retain) NewsArticle * newsArticle;
@property (nonatomic, retain) Symbol * symbol;

@end



