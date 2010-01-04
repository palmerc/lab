//
//  Symbol.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Symbol : NSObject {
	NSNumber *feedNumber;
	NSString *ticker;
	NSString *name;
	NSString *isin; // International Securities Identification Number
	NSNumber *type;
	NSString *orderbook;
	NSNumber *exchangeCode;
}

@property (nonatomic, retain) NSNumber *feedNumber;
@property (nonatomic, retain) NSString *ticker;
@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString *isin;
@property (nonatomic, retain) NSNumber *type;
@property (nonatomic, retain) NSString *orderbook;
@property (nonatomic, retain) NSNumber *exchangeCode;

@end
