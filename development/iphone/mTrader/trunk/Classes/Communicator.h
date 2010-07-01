//
//  Communicator.h
//  Simple Asynchronous Networking - This class should only define a simple method for
//    connecting to an arbitrary socket. It shouldn't assume anything.
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2010 Infront AS. All rights reserved.
//

@protocol CommunicatorStatusDelegate;
@protocol CommunicatorDataDelegate;

@interface Communicator : NSObject /*<NSStreamDelegate>*/ {
@private
	id <CommunicatorStatusDelegate> _statusDelegate;
	id <CommunicatorDataDelegate> _dataDelegate;
	
	NSString *_host;
	NSUInteger _port;
	
	NSInputStream *_inputStream;
	NSOutputStream *_outputStream;
	
	NSMutableData *_inboundBuffer;
	NSMutableData *_outboundBuffer;
		
	NSUInteger _bytesReceived;
	NSUInteger _bytesSent;
}

@property (nonatomic, assign) id <CommunicatorStatusDelegate> statusDelegate;
@property (nonatomic, assign) id <CommunicatorDataDelegate> dataDelegate;

@property (readonly) NSString *host;
@property (readonly) NSUInteger port;
@property (readonly) NSUInteger bytesReceived;
@property (readonly) NSUInteger bytesSent;

- (void)startConnectionWithSocket:(NSString *)host onPort:(NSUInteger)port;
- (void)stopConnection;

- (void)sendData:(NSData *)data;
@end

@protocol CommunicatorStatusDelegate <NSObject>
- (void)connect;
- (void)disconnect;
@end

@protocol CommunicatorDataDelegate <NSObject>
- (void)receivedData:(NSData *)data;
@end
