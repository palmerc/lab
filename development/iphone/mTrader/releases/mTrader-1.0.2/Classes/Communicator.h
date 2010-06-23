//
//  Communicator.h
//  simpleNetworking - This class should only define a simple method for
//    connecting to an arbitrary socket. It shouldn't assume anything.
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

@protocol CommunicatorDataDelegate;

@interface Communicator : NSObject <NSStreamDelegate> {
	id <CommunicatorDataDelegate> _delegate;
	
	NSString *_host;
	NSInteger _port;
	
	NSInputStream *_inputStream;
	NSOutputStream *_outputStream;
	
	NSMutableData *_dataBuffer;
	NSMutableArray *_lineBuffer;
	NSMutableArray *_blockBuffer;
	
	int previousByte;
	BOOL _isConnected;
}

@property (nonatomic, assign) id <CommunicatorDataDelegate> delegate;

@property (nonatomic, retain) NSString *host;
@property NSInteger port;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (nonatomic, retain) NSMutableData *dataBuffer;
@property (nonatomic, retain) NSMutableArray *lineBuffer;
@property (nonatomic, retain) NSMutableArray *blockBuffer;

@property BOOL isConnected;

- (id)initWithSocket:(NSString *)host onPort:(NSInteger)port;
- (void)startConnection;
- (void)stopConnection;
- (void)processBuffer;
- (void)processLines;
- (void)dataReceived:(NSInputStream *)stream;
- (void)writeString:(NSString *)string;
@end

@protocol CommunicatorDataDelegate <NSObject>
- (void)dataReceived:(NSArray *)lineBuffer;
- (void)connected;
- (void)disconnected;
@end

