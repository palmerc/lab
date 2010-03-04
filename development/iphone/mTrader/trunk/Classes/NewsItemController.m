//
//  NewsItemController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NewsItemController.h"

#import "SymbolNewsItemView.h"
#import "StringHelpers.h"

@implementation NewsItemController
@synthesize feedArticle = _feedArticle;

- (id)init {
	self = [super init];
	if (self != nil) {
		_feedArticle = nil;
		newsItemView = nil;
	}
	return self;
}

- (void)loadView {
	CGRect frame = self.parentViewController.view.bounds;
	newsItemView = [[SymbolNewsItemView alloc] initWithFrame:frame];
	self.view = newsItemView;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	communicator.symbolsDelegate = self;
	[communicator newsItemRequest:self.feedArticle];
	
    [super viewDidLoad];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

#pragma mark -
#pragma mark Delegation

- (void)newsItemUpdate:(NSArray *)newsItemContents {
	newsItemContents = [StringHelpers cleanComponents:newsItemContents];
	NSString *feedArticle = [newsItemContents objectAtIndex:0];
	NSString *dateTime = [newsItemContents objectAtIndex:1];
	NSString *flags = [newsItemContents objectAtIndex:2];
	NSString *headline = [newsItemContents objectAtIndex:3];
	NSString *body = [newsItemContents objectAtIndex:4];
	
	NSArray *feedArticleComponents = [feedArticle componentsSeparatedByString:@"/"];
	NSString *feed = [feedArticleComponents objectAtIndex:0];
	
	body = [body stringByReplacingOccurrencesOfString:@"||" withString:@"\n"];
	body = [StringHelpers cleanString:body];
	
	self.title = feed;
	newsItemView.dateTimeLabel.text = dateTime;
	newsItemView.feedLabel.text = feed;
	newsItemView.flags = flags;
	newsItemView.headlineLabel.text = headline;
	newsItemView.bodyLabel.text = body;
	
	[newsItemView setNeedsLayout];
}
/*
#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in NewsItemController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
*/
#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_feedArticle release];
	[newsItemView release];
    [super dealloc];
}


@end
