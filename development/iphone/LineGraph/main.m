//
//  main.m
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 27.02.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//


int main(int argc, char *argv[]) {
    static NSString *appDelegate = @"LineGraphAppDelegate";
	
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, appDelegate);
    [pool release];
    return retVal;
}
