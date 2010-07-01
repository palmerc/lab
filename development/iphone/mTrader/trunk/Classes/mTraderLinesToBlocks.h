//
//  mTraderLinesToBlocks.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "LineOrientedCommunication.h"

@protocol mTraderBlockDataDelegate;

@interface mTraderLinesToBlocks : NSObject <LineOrientedDataDelegate> {
	id <mTraderBlockDataDelegate> _dataDelegate;
	NSMutableArray *_blockBuffer;
}

@property (nonatomic, assign) id <mTraderBlockDataDelegate> dataDelegate;

@end

@protocol mTraderBlockDataDelegate <NSObject>
- (void)receivedDataBlock:(NSArray *)block;
@end