    //
//  CPViewController.m
//  ResizeElement
//
//  Created by Cameron Lowell Palmer on 28.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "CPViewController.h"
#import "CPView.h"

@implementation CPViewController


- (void)loadView {
	CGRect screenFrame = [[UIScreen mainScreen] applicationFrame];
	_cpView = [[CPView alloc] initWithFrame:screenFrame];
	
	self.view = _cpView;
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	unichar poiCode = 0x2318;
	NSString *poi = [NSString stringWithCharacters:&poiCode length:1];
	_cpView.helloLabel.text = [NSString stringWithFormat:@"%@ %@", poi, @"Hello, World!"];
	[_cpView.helloLabel setFont:[UIFont systemFontOfSize:32.0f]];
		
	UIBarButtonItem *navBarItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(add:)];
	self.navigationItem.rightBarButtonItem = navBarItem;
	[navBarItem release];

	UIBarButtonItem *navBarItemToo = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAction target:self action:@selector(action:)];
	self.navigationItem.leftBarButtonItem = navBarItemToo;
	[navBarItemToo release];
	
}

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return YES;
}

- (void)add:(id)sender {
	UIView *topBar = _cpView.topBar;
	if ([topBar isHidden]) {
		_cpView.topBar.hidden = NO;
	} else {
		_cpView.topBar.hidden = YES;
	}
}

- (void)action:(id)sender {
	if ([_cpView isShrunk]) {
		[_cpView stretch];
	} else {
		[_cpView shrink];
	}
}

- (void)dealloc {
	[_cpView release];
	
	[super dealloc];
}

@end
