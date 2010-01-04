//
//  Customer.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface UserDefaults : NSObject {
	NSString *username;
	NSString *password;
}

+ (UserDefaults *)sharedManager;

@property (nonatomic, retain) NSString *username;
@property (nonatomic, retain) NSString *password;

@end
