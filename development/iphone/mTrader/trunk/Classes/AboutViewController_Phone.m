//
//  AboutViewController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 27.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//
#import "AboutViewController_Phone.h"

#import "NetworkDiagnosticViewController_Phone.h"

@implementation AboutViewController_Phone
@synthesize versionTextField;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		self.title = NSLocalizedString(@"About", @"About string");
	}
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	NSString *version = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
	self.versionTextField.text = [NSString stringWithFormat:@"v%@", version];
}

- (IBAction)buttonAction {
	NetworkDiagnosticViewController_Phone *netDiagVC = [[NetworkDiagnosticViewController_Phone alloc] init];
	[self.navigationController pushViewController:netDiagVC animated:YES];
	[netDiagVC release];
}

- (void)dealloc {
    [super dealloc];
}


@end
