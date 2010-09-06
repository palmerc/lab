//
//  Feed.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Symbol;

@interface Feed :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * decimals;
@property (nonatomic, retain) NSString * feedNumber;
@property (nonatomic, retain) NSString * feedName;
@property (nonatomic, retain) NSString * mCode;
@property (nonatomic, retain) NSString * typeCode;
@property (nonatomic, retain) NSSet* symbols;

@end


@interface Feed (CoreDataGeneratedAccessors)
- (void)addSymbolsObject:(Symbol *)value;
- (void)removeSymbolsObject:(Symbol *)value;
- (void)addSymbols:(NSSet *)value;
- (void)removeSymbols:(NSSet *)value;

@end

