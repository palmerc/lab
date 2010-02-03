//
//  NewsItemViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 22.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//


#import "NewsItemViewController.h"
#import "mTraderCommunicator.h"

@implementation NewsItemViewController
@synthesize newsItemID = _newsItemID;
@synthesize date, time, headline, body;

#pragma mark Lifecycle

- (id)initWithNewsItem:(NSString *)newsItemID {
	self = [super init];
	if (self != nil) {
		self.newsItemID = newsItemID;
		
		mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
		communicator.newsItemDelegate = self;
		[communicator newsItemRequest:newsItemID];
		
		self.hidesBottomBarWhenPushed = YES;
		self.title = newsItemID;
	}
	
	return self;
}

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	UIFont *font = [UIFont fontWithName:@"Courier" size:14.0];
	NSMutableString *stringToMeasure = [[NSMutableString alloc] init];
	for (int i = 0; i < 80; i++) {
		[stringToMeasure appendString:@"X"];
	}
	sizeOfLine = [stringToMeasure sizeWithFont:font];
	[stringToMeasure release];
}



// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return YES;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
	NSLog(@"Rotation %@ %@", toInterfaceOrientation, duration);
}

#pragma mark Delegation

-(void) newsItemUpdate:(NSArray *)newItemContents {
	UIFont *font = [UIFont fontWithName:@"Courier" size:14];
	self.time.text = [newItemContents objectAtIndex:1];
	self.time.font = font;
	
	self.headline.text = [newItemContents objectAtIndex:3];
	self.headline.numberOfLines = 0;
	self.headline.font = font;
	
	NSMutableArray *cleanedTextLines = [[NSMutableArray alloc] init];
	NSString *bodyText = [newItemContents objectAtIndex:4];
	NSArray *dirtyTextLines = [bodyText componentsSeparatedByString:@"||"];
	for (NSString *line in dirtyTextLines) {
		[cleanedTextLines addObject:[self cleanString:line]];
	}
	if ([[cleanedTextLines objectAtIndex:0] isEqualToString:@""]) {
		[cleanedTextLines removeObjectAtIndex:0];
	}
	self.body.text = [cleanedTextLines componentsJoinedByString:@"\n"];
	self.body.font = font;
		
	[cleanedTextLines release];
}
	 
- (NSString *)cleanString:(NSString *)string {
	NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
	
	NSString *aCleanString = [string stringByTrimmingCharactersInSet:whitespaceAndNewline];
	
	return aCleanString;
}

@end
