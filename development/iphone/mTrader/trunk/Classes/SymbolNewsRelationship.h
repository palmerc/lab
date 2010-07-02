//
//  SymbolNewsRelationship.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class NewsArticle;
@class Symbol;

@interface SymbolNewsRelationship :  NSManagedObject  
{
}

@property (nonatomic, retain) Symbol * symbol;
@property (nonatomic, retain) NewsArticle * newsArticle;

@end



