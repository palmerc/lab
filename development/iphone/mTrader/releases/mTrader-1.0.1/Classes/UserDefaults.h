//
//  UserDefaults.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//
@interface UserDefaults : NSObject {
	NSString *username;
	NSString *password;
}

+ (UserDefaults *)sharedManager;
- (void)saveSettings;

@property (nonatomic, retain) NSString *username;
@property (nonatomic, retain) NSString *password;

@end
