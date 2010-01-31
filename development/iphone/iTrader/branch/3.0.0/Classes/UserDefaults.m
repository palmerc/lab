//
//  Customer.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "UserDefaults.h"


@implementation UserDefaults
@synthesize username;
@synthesize password;

static UserDefaults *sharedDefaults = nil;

#pragma mark Initialization and Cleanup
- (id)init {
	self = [super init];
	if (self != nil) {
		NSUserDefaults *defaults = [[NSUserDefaults alloc] init];
		
		self.username = [defaults stringForKey:@"username"];
		self.password = [defaults stringForKey:@"password"];
		//NSLog(@"user: %@, password: %@", self.username, self.password);
		
		[defaults release];
	}
	
	return self; 
}

- (void)dealloc {
	[sharedDefaults release];
	[self.username release];
	[self.password release];
	[super dealloc];
}

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

#pragma mark Save User Defaults
- (void)saveSettings {
	NSUserDefaults *defaults = [[NSUserDefaults alloc] init];
	[defaults setObject:self.username forKey:@"username"];
	[defaults setObject:self.password forKey:@"password"];
}

@end
