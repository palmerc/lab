//
//  Communicator.h
//  simpleNetworking - This class should only define a simple method for
//    connecting to an arbitrary socket. It shouldn't assume anything.
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 Bird And Bear Productions. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Communicator : NSObject {
	NSString *_host;
	NSInteger _port;
	
	NSInputStream *_inputStream;
	NSOutputStream *_outputStream;
	
	NSMutableData *_inputBuffer;
	NSMutableData *_outputBuffer;
	
	BOOL _isConnected;
	
	NSNumber *_bytesRead;
	int _byteIndex;
}

@property (nonatomic, retain) NSString *host;
@property NSInteger port;

@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;

@property (nonatomic, retain) NSMutableData *inputBuffer;
@property (nonatomic, retain) NSMutableData *outputBuffer;

@property BOOL isConnected;
@property (retain) NSNumber *bytesRead;
@property int byteIndex;

- (id)initWithSocket:(NSString *)host port:(NSInteger)port;

- (void)startConnection;
- (void)stopConnection;

- (void)write:(NSData *)data;
- (NSData *)read;
- (void)test;

@end
