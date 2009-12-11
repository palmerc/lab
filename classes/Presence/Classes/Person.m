//
//  Person.m
//  Presence
//
//  Created by Cameron Lowell Palmer on 10.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "Person.h"


@implementation Person

@synthesize name;
@synthesize status;
@synthesize image;

- (id)init {
	self = [super init];
	if (self) {
	}	
    return self;
}

- (id)initWithName:(NSString *)name {
	self = [super init];
	if (self) {
		self.name = name;
	}	
    return self;
}

- (void)dealloc {
    [super dealloc];
}

+ (NSArray *)initWithNames:(NSString *)firstObj, ... NS_REQUIRES_NIL_TERMINATION {

}

@end
