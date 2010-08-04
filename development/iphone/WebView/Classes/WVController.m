//
//  WVController.m
//  WebView
//
//  Created by Cameron Lowell Palmer on 04.08.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "WVController.h"


@implementation WVController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	CGRect applicationFrame = [[UIScreen mainScreen] applicationFrame];
	UIView *aView = [[UIView alloc] initWithFrame:applicationFrame];
	
	_webView = [[UIWebView alloc] initWithFrame:aView.bounds];
	_webView.delegate = self;
	_webView.scalesPageToFit = YES;
	_webView.backgroundColor = [UIColor yellowColor];
	_webView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	_webView.dataDetectorTypes = UIDataDetectorTypeAll;
	
	[aView addSubview:_webView];	
	
	self.view = aView;
	[aView release];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	self.title = @"Browser";
	
	unichar backArrowCode = 0x25C0;
	NSString *backArrowString = [NSString stringWithCharacters:&backArrowCode length:1];
	UIBarButtonItem *backBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:backArrowString style:UIBarButtonItemStylePlain target:self action:@selector(backBarButtonItem:)];
	self.navigationItem.leftBarButtonItem = backBarButtonItem;
	
	NSURL *baseURL = [NSURL URLWithString:@"http://localhost"];
	[_webView loadHTMLString:@"<html><head><title>Test</title></head><body><p><a href=\"http://news.google.com\">Hello, world</a></p><p>http://www.google.com. +47 95 77 00 89 or (214) 495-2097 alternatively (800) GOOG-411</p></body></html>" baseURL:baseURL];	
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
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
	_webView.delegate = nil;
}

- (void)backBarButtonItem:(id)sender {
	[_webView goBack];	
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
	NSLog(@"%@ %@ %d", webView, request, navigationType);
	
	return YES;
}

- (void)webViewDidStartLoad:(UIWebView *)webView {
	NSLog(@"%@", webView);
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
	NSLog(@"%@", webView);
}

- (void)dealloc {
	[_webView release];
	
    [super dealloc];
}


@end
