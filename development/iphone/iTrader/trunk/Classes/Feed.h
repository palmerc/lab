//
//  Feed.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 29.01.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Symbol;

@interface Feed :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * mCode;
@property (nonatomic, retain) NSString * description;
@property (nonatomic, retain) NSNumber * feedNumber;
@property (nonatomic, retain) NSSet* symbols;

@end


@interface Feed (CoreDataGeneratedAccessors)
- (void)addSymbolsObject:(Symbol *)value;
- (void)removeSymbolsObject:(Symbol *)value;
- (void)addSymbols:(NSSet *)value;
- (void)removeSymbols:(NSSet *)value;

@end

