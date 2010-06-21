//
//  Communicator.h
//  simpleNetworking - This class should only define a simple method for
//    connecting to an arbitrary socket. It shouldn't assume anything.
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

@protocol CommunicatorDataDelegate;

@interface Communicator : NSObject {
@private
	id <CommunicatorDataDelegate> delegate;
	
	NSString *_host;
	NSInteger _port;
	
	NSInputStream *_inputStream;
	NSOutputStream *_outputStream;
	NSMutableArray *_lineBuffer;
	NSMutableData *_mutableDataBuffer;
	
	NSUInteger _bytesReceived;
	NSUInteger _bytesSent;
}

@property (nonatomic, assign) id <CommunicatorDataDelegate> delegate;
@property (nonatomic, retain) NSString *host;
@property NSInteger port;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (nonatomic, retain) NSMutableArray *lineBuffer;
@property (nonatomic, retain) NSMutableData *mutableDataBuffer;
- (void)readAvailableBytes:(NSInputStream *)aStream;
- (void)dataReceived:(NSData *)dataBlock;
@property (readonly) NSUInteger bytesReceived;
@property (readonly) NSUInteger bytesSent;

- (id)initWithSocket:(NSString *)host onPort:(NSInteger)port;
- (void)startConnection;
- (void)stopConnection;
- (void)writeString:(NSString *)string;
- (NSData *)readLine;
@end

@protocol CommunicatorDataDelegate <NSObject>
- (void)dataReceived;
- (void)connected;
- (void)disconnected;
@end

