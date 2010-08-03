//
//  UserDefaults.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//
@interface UserDefaults : NSObject {
@private
	NSString *_username;
	NSString *_password;
	
	NSString *_newsFeedNumber;
	NSData *_deviceToken;
}

+ (UserDefaults *)sharedManager;
- (void)saveSettings;

@property (nonatomic, retain) NSString *username;
@property (nonatomic, retain) NSString *password;
@property (nonatomic, retain) NSString *newsFeedNumber;
@property (nonatomic, retain) NSData *deviceToken;

@end
