//
//  CPHost.m
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 12.04.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "CPHost.h"

#import <arpa/inet.h>
#import <ifaddrs.h>

@implementation CPHost

+ (NSDictionary *)interfacesToAddresses {
	NSMutableDictionary *interfacesToAddresses = [NSMutableDictionary dictionary];
	struct ifaddrs *interfaces = NULL;
	struct ifaddrs *currentAddress = NULL;
	
	int success = getifaddrs(&interfaces);
	if (success == 0) {
		currentAddress = interfaces;
		while(currentAddress != NULL) {
			if(currentAddress->ifa_addr->sa_family == AF_INET) {
				NSString *interface = [NSString stringWithUTF8String:currentAddress->ifa_name];
				NSString *address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)currentAddress->ifa_addr)->sin_addr)];
				[interfacesToAddresses setObject:address forKey:interface];
			}
			currentAddress = currentAddress->ifa_next;
		}
	}
	freeifaddrs(interfaces);
	return interfacesToAddresses;
}

@end
