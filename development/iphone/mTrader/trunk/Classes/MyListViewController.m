//
//  MyListViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "MyListViewController.h"

#import "ChainsTableViewController.h"

@implementation MyListViewController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize tableViewController = _tableViewController;
@synthesize navigationController = _navigationController; 

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
	}
	return self;
}

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	[super loadView];
		
	CGRect frame = self.view.bounds;
	UIView *aView = [[UIView alloc] initWithFrame:frame];
	
	aView.backgroundColor = [UIColor whiteColor];
	
	
	UIImage *mTraderImage = [UIImage imageNamed:@"Mtrader_16.png"];
	UIImageView *mTraderBranding = [[UIImageView alloc] initWithFrame:CGRectMake(10.0f, 10.0f, 112.0f, 16.0f)];
	mTraderBranding.image = mTraderImage;
	
	_tableViewController = [[ChainsTableViewController alloc] initWithManagedObjectContext:self.managedObjectContext];
	
	CGRect buttonFrame = CGRectMake(0.0f, 2.0f, 85.0f, 37.0f);
	buttonFrame.origin.x = frame.size.width - 85.0f * 2.0f - 4.0f;
	UIButton *centerButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[centerButton setTitle:@"Last" forState:UIControlStateNormal];
	centerButton.frame = buttonFrame;
	centerButton.backgroundColor = [UIColor whiteColor];
	[centerButton addTarget:self.tableViewController action:@selector(centerSelection:) forControlEvents:UIControlEventTouchUpInside];
	
	buttonFrame.origin.x = frame.size.width - 85.0f - 2.0f;
	UIButton *rightButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[rightButton setTitle:@"%" forState:UIControlStateNormal];
	rightButton.frame = buttonFrame;
	rightButton.backgroundColor = [UIColor whiteColor];
	[rightButton addTarget:self.tableViewController action:@selector(rightSelection:) forControlEvents:UIControlEventTouchUpInside];
	
	frame.origin.y = 39.0f;
	frame.size.height -= 39.0f;
	self.tableViewController.view.frame = frame;
	
	[aView addSubview:mTraderBranding];
	[aView addSubview:centerButton];
	[aView addSubview:rightButton];
	[aView addSubview:self.tableViewController.view];
	
	self.view = aView;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	self.title = NSLocalizedString(@"ChainsTab", @"Chains tab label");

	//CGRect buttonFrame = CGRectMake(0.0f, 0.0f, 85.0f, 37.0f);
	
	self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemEdit target:self.tableViewController action:@selector(setEditing:animated:)];
	
	// Setup right and left bar buttons
	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(add:)];
	self.navigationItem.rightBarButtonItem = addItem;
	[addItem release];
	
	self.tableViewController.navigationController = self.navigationController;
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

- (void)failedToAddAlreadyExists {
	NSString *alertTitle = @"Add Security Failed";
	NSString *alertMessage = @"The ticker symbol you requested is already in your list.";
	NSString *alertCancel = @"Dismiss";
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertTitle message:alertMessage delegate:nil cancelButtonTitle:alertCancel otherButtonTitles:nil];
	[alertView show];	
}

- (void)failedToAddNoSuchSecurity {
	NSString *alertTitle = @"Add Security Failed";
	NSString *alertMessage = @"The ticker symbol you requested was not found.";
	NSString *alertCancel = @"Dismiss";
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertTitle message:alertMessage delegate:nil cancelButtonTitle:alertCancel otherButtonTitles:nil];
	[alertView show];
}


#pragma mark -
#pragma mark UI Actions
- (void)add:(id)sender {
	SymbolAddController *controller = [[SymbolAddController alloc] initWithNibName:@"SymbolAddView" bundle:nil];
	controller.delegate = self;
	controller.managedObjectContext = self.managedObjectContext;
	
	UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:controller];
	[controller release];
	[self.parentViewController presentModalViewController:navigationController animated:YES];
	[navigationController release];
}

- (void)symbolAddControllerDidFinish:(SymbolAddController *)stockSearchController didAddSymbol:(NSString *)tickerSymbol {
	[self dismissModalViewControllerAnimated:YES];
}



- (void)dealloc {
	[_managedObjectContext release];
	[_tableViewController release];
	
    [super dealloc];
}


@end
