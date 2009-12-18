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
	NSString *host;
	NSInteger port;
	NSInputStream *inputStream;
	NSOutputStream *outputStream;
	BOOL isConnected;
}

@property (nonatomic, retain) NSString *host;
@property NSInteger port;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (readonly) BOOL isConnected;

- (id)initWithSocket:(NSString *)host port:(NSInteger)port;
- (void)write:(NSString *)string;
- (NSString *)read;

@end
