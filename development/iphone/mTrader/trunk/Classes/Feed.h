//
//  Feed.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 02.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Symbol;

@interface Feed :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * feedNumber;
@property (nonatomic, retain) NSString * description;
@property (nonatomic, retain) NSString * mCode;
@property (nonatomic, retain) NSSet* symbols;

@end


@interface Feed (CoreDataGeneratedAccessors)
- (void)addSymbolsObject:(Symbol *)value;
- (void)removeSymbolsObject:(Symbol *)value;
- (void)addSymbols:(NSSet *)value;
- (void)removeSymbols:(NSSet *)value;

@end

