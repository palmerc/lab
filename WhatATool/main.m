//
//  main.m
//  WhatATool
//
//  Created by Cameron Lowell Palmer on 19.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "PolygonShape.h"

void Section1() {
	NSString *path = @"~";
	path = [path stringByExpandingTildeInPath];
	
	NSString *message = [[NSString alloc] initWithFormat:@"My home folder is at %@", path];
	NSLog(@"%@", message);
	[message release];
	
	NSArray *array = [path pathComponents];
	for (NSString *part in array) {
		NSLog(@"%@", part);
	}
	[path release];
}

void Section2() {
	NSString *processName = [[NSProcessInfo processInfo] processName];
	NSNumber *processId = [NSNumber numberWithInt:[[NSProcessInfo processInfo] processIdentifier]];
	NSString *processMessage = [[NSString alloc] initWithFormat:@"Process Name: '%@' Process ID: '%@'", processName, processId];
	
	NSLog(@"%@", processMessage);
	[processMessage release];
}

void Section3() {
	NSArray *keys = [NSArray arrayWithObjects:@"Stanford University", 
					 @"Apple", 
					 @"CS193P", 
					 @"Stanford on iTunes U",
					 @"Stanford Mall", 
					 nil];
	NSArray *values = [NSArray arrayWithObjects:[NSURL URLWithString:@"http://www.stanford.edu"], 
					   [NSURL URLWithString:@"http://www.apple.com"], 
					   [NSURL URLWithString:@"http://cs193p.stanford.edu"], 
					   [NSURL URLWithString:@"http://itunes.stanford.edu"], 
					   [NSURL URLWithString:@"http://stanfordshop.com"],
					   nil];
	
	NSDictionary *dictionary = [NSDictionary dictionaryWithObjects:values forKeys:keys];
	
	for (id key in dictionary) {
		NSRange range = [key rangeOfString:@"Stanford"];
		if (range.location == 0)
			NSLog(@"Key: %@, URL: %@", key, [dictionary objectForKey:key]);
	}
}

void Section4() {
	NSMutableArray *mutable = [NSMutableArray arrayWithObjects:nil];
	NSArray *keys = [NSArray arrayWithObjects:@"Stanford University", 
					 @"Apple", 
					 @"CS193P", 
					 @"Stanford on iTunes U",
					 @"Stanford Mall", 
					 nil];
	NSArray *values = [NSArray arrayWithObjects:[NSURL URLWithString:@"http://www.stanford.edu"], 
					   [NSURL URLWithString:@"http://www.apple.com"], 
					   [NSURL URLWithString:@"http://cs193p.stanford.edu"], 
					   [NSURL URLWithString:@"http://itunes.stanford.edu"], 
					   [NSURL URLWithString:@"http://stanfordshop.com"],
					   nil];
	
	[mutable addObject:[NSDictionary dictionaryWithObjects:values forKeys:keys]];
	NSProcessInfo *processInfo = [[NSProcessInfo alloc] init];
	NSString *processName = [processInfo processName];
	NSNumber *processId = [NSNumber numberWithInt:[[NSProcessInfo processInfo] processIdentifier]];
	[mutable addObject:[NSURL URLWithString:@"http://stanfordshop.com"]];
	[mutable addObject:processInfo];
	[mutable addObject:processName];
	[mutable addObject:processId];
	[mutable addObject:@"TESTING"];
	
	for (id object in mutable) {
		NSLog(@"Class name: %@", [object class]);
		if ([object isMemberOfClass:[NSString class]] == YES) {
			NSLog(@"Is Member of NSString: YES");
		} else {
			NSLog(@"Is Member of NSString: NO");
		}

		if ([object isKindOfClass:[NSString class]] == YES) {
			NSLog(@"Is Kind of NSString: YES");
		} else {
			NSLog(@"Is Kind of NSString: NO");
		}

		if ([object respondsToSelector: @selector( lowercaseString )] == YES) {
			NSLog(@"lowercaseString is: %@", [object lowercaseString]);
		} else {
			NSLog(@"Responds to lowercaseString: NO");
		}
		NSLog(@"=================================================");
		
	}
}

void PrintPolygonInfo() {
	NSMutableArray *array = [[NSMutableArray alloc] init];
	
	[array addObject:[[PolygonShape alloc] initWithNumberOfSides:4 minimumNumberOfSides:3 maximumNumberOfSides:7]];
	[array addObject:[[PolygonShape alloc] initWithNumberOfSides:6 minimumNumberOfSides:5 maximumNumberOfSides:9]];
	[array addObject:[[PolygonShape alloc] initWithNumberOfSides:12 minimumNumberOfSides:9 maximumNumberOfSides:12]];
	
	for (id object in array) {
		NSLog(@"%@", [object description]);
	}
	
	for (id object in array) {
		[object setNumberOfSides:10];
	}
	
	for (id object in array) {
		[object release];
	}
	
	[array release];
	
}

int main (int argc, const char * argv[]) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	Section1();
	Section2();
	Section3();
	Section4();
	PrintPolygonInfo();
	[pool release];
}
