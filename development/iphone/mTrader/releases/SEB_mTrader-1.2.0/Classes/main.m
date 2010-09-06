//
//  main.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright Infront AS 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	// For now we aren't using this
	BOOL iPad = NO;
#ifdef UI_USER_INTERFACE_IDIOM
//	iPad = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
#endif
	
	NSString *delegateClassName = nil;
	if (iPad) {
		delegateClassName = @"mTraderAppDelegate_Pad";
	} else {
		delegateClassName = @"mTraderAppDelegate_Phone";
	}
	
    int retVal = UIApplicationMain(argc, argv, nil, delegateClassName);
	[pool release];
	
	return retVal;
}
