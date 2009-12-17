//
//  Communicator.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

enum {
    kSendBufferSize = 32768
};

@interface Communicator : NSObject {
	NSString *username;
	NSString *password;
	NSNetService *netService;
	NSInputStream *inputStream;
	NSOutputStream *outputStream;
	uint8_t buffer[kSendBufferSize];
	BOOL isConnected;
}

@property (assign) NSString *username;
@property (assign) NSString *password;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;
@property (nonatomic) BOOL isConnected;

- (id)initWithUsernameAndPassword:(NSString *)_username password:(NSString *)_password;
- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode;
- (void)setup;
- (BOOL)login;

@end
