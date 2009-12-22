//
//  Communicator.h
//  simpleNetworking - This class should only define a simple method for
//    connecting to an arbitrary socket. It shouldn't assume anything.
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Bird And Bear Productions. All rights reserved.
//

#import <Foundation/Foundation.h>

@class queue;
@protocol CommunicatorReceiveDelegate;

@interface Communicator : NSObject {
	id <CommunicatorReceiveDelegate> delegate;
	
	NSString *_host;
	NSInteger _port;
	
	NSInputStream *_inputStream;
	NSOutputStream *_outputStream;
	NSMutableData *_dataBuffer;
	NSMutableArray *_lineBuffer;
	
	BOOL _isConnected;
}

@property (nonatomic, assign) id <CommunicatorReceiveDelegate> delegate;
@property (nonatomic, retain) NSString *host;
@property NSInteger port;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (nonatomic, retain) NSMutableData *dataBuffer;
@property (nonatomic, retain) NSMutableArray *lineBuffer;

@property BOOL isConnected;

- (id)initWithSocket:(NSString *)host port:(NSInteger)port;
- (void)startConnection;
- (void)stopConnection;
- (void)writeString:(NSString *)string;
- (NSString *)readLine;
@end

@protocol CommunicatorReceiveDelegate <NSObject>
- (void)dataReceived;
@end

