//
//  UserDefaults.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "UserDefaults.h"


@implementation UserDefaults
@synthesize username = _username;
@synthesize password = _password;
@synthesize newsFeedNumber = _newsFeedNumber;
@synthesize deviceToken = _deviceToken;

static UserDefaults *sharedDefaults = nil;

#pragma mark Singleton Methods
+ (UserDefaults *)sharedManager {
	if (sharedDefaults == nil) {
		sharedDefaults = [[super allocWithZone:NULL] init];
	}
	return sharedDefaults;
}

+ (id)allocWithZone:(NSZone *)zone {
	return [[self sharedManager] retain];
}

- (id)copyWithZone:(NSZone *)zone {
	return self;
}

- (id)retain {
	return self;
}

- (NSUInteger)retainCount {
	return NSUIntegerMax;
}

- (void)release {
	// do nothing
}

- (id)autorelease {
	return self;
}

#pragma mark Initialization
- (id)init {
	self = [super init];
	if (self != nil) {
		NSUserDefaults *defaults = [[NSUserDefaults alloc] init];
		
		_username = [[defaults stringForKey:@"username"] retain];
		_password = [[defaults stringForKey:@"password"] retain];
		_newsFeedNumber = [[defaults stringForKey:@"newsFeedNumber"] retain];
		_deviceToken = [[defaults dataForKey:@"deviceToken"] retain];
		
		[defaults release];
	}
	
	return self; 
}

#pragma mark Save User Defaults
- (void)saveSettings {
	NSUserDefaults *defaults = [[NSUserDefaults alloc] init];
	[defaults setObject:_username forKey:@"username"];
	[defaults setObject:_password forKey:@"password"];
	[defaults setObject:_newsFeedNumber forKey:@"newsFeedNumber"];
	[defaults setObject:_deviceToken forKey:@"deviceToken"];
	[defaults release];
}

- (void)dealloc {
	[sharedDefaults release];
	[_username release];
	[_password release];
	[_newsFeedNumber release];
	[_deviceToken release];
	
	[super dealloc];
}

@end
