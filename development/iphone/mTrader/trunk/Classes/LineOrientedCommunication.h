//
//  LineOrientedCommunication.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "Communicator.h"


@protocol LineOrientedDataDelegate;

@interface LineOrientedCommunication : NSObject <CommunicatorDataDelegate> {
@private
	id <LineOrientedDataDelegate> _dataDelegate;
	
	NSMutableData *_dataBuffer;
}

@property (nonatomic, assign) id <LineOrientedDataDelegate> dataDelegate;

@end

@protocol LineOrientedDataDelegate <NSObject>
- (void)receivedDataLine:(NSData *)data;
@end
