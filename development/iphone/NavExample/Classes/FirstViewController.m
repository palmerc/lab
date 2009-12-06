//
//  FirstViewController.m
//  NavExample
//
//  Created by Cameron Lowell Palmer on 05.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "FirstViewController.h"
#import "SecondViewController.h"


@implementation FirstViewController

- (IBAction)push:(id)sender {
	NSLog(@"push");
	SecondViewController *secondViewController = [[SecondViewController alloc] initWithText:@"What! What!"];
	[self.navigationController pushViewController:secondViewController animated:YES];
	[secondViewController release];
}

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		NSLog(@"The initWithNibName");
		self.title = @"First VC";
    }
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];

	UIBarButtonItem *backBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"First" style:UIBarButtonItemStyleBordered target:nil action:nil];
	self.navigationItem.backBarButtonItem = backBarButtonItem;
	[backBarButtonItem release];
	
	UIBarButtonItem *composeBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCompose target:self action:@selector(add:)];
	self.navigationItem.rightBarButtonItem = composeBarButtonItem;
	[composeBarButtonItem release];
}

- (void)add:(id)sender {
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Unable To Compose" message:@"Since this wasn't implemented it doesn't work." delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles:nil];
	[alertView show];
	[alertView release];
}

- (void)dealloc {
    [super dealloc];
}


@end
