//
//  LineGraphAppDelegate.m
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 27.02.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//


#import "LineGraphAppDelegate.h"

#import "LineGraphController.h" 
#import "XMLParser.h"

#import "Helper.h"

@implementation LineGraphAppDelegate
@synthesize window;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	NSString *path = [[NSBundle mainBundle] pathForResource:@"marketInfo" ofType:@"xml"];
	NSURL *url = [[NSURL alloc] initFileURLWithPath:path];
	NSXMLParser *xmlParser = [[NSXMLParser alloc] initWithContentsOfURL:url];
	
	XMLParser *theHorse = [[XMLParser alloc] init];
	
	xmlParser.delegate = theHorse;
	[xmlParser parse];
	
	NSArray *points = [Helper graphPointsFromDataPoints:theHorse.points];
	
	window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	window.backgroundColor = [UIColor lightGrayColor];
	
	CGRect frame = [[UIScreen mainScreen] bounds];
	controller = [[LineGraphController alloc] initWithFrame:frame];
	controller.points = points;
	[window addSubview:controller.view];
	
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[controller release];
	
    [window release];
    [super dealloc];
}

@end
