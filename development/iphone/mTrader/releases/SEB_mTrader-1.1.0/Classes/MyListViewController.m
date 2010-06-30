//
//  MyListViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "MyListViewController.h"

#import "ChainsTableViewController.h"
#import "QFields.h"

@implementation MyListViewController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize tableViewController = _tableViewController;
@synthesize navigationController = _navigationController; 

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		_managedObjectContext = [managedObjectContext retain];
		_tableViewController = [[ChainsTableViewController alloc] initWithManagedObjectContext:managedObjectContext];
	}
	return self;
}

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	[super loadView];
		
	CGRect frame = self.view.bounds;
	UIView *aView = [[UIView alloc] initWithFrame:frame];
	
	aView.backgroundColor = [UIColor whiteColor];
	
	UIImage *mTraderImage = [UIImage imageNamed:@"mTrader.png"];
	UIImageView *mTraderBranding = [[UIImageView alloc] initWithFrame:CGRectMake(10.0f, 10.0f, 112.0f, 16.0f)];
	mTraderBranding.image = mTraderImage;
	
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
	[mTraderBranding release];

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
	self.navigationItem.leftBarButtonItem = self.editButtonItem;	
	
	// Setup right and left bar buttons
	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(add:)];
	self.navigationItem.rightBarButtonItem = addItem;
	[addItem release];
}

- (void)viewDidAppear:(BOOL)animated {
	[super viewDidAppear:animated];

	[self changeQFieldsStreaming];
}

- (void)changeQFieldsStreaming {
	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	
	QFields *qFields = [[QFields alloc] init];
	qFields.timeStamp = YES;
	qFields.lastTrade = YES;
	qFields.bidPrice = YES;
	qFields.askPrice = YES;
	qFields.change = YES;
	qFields.changePercent = YES;
	qFields.changeArrow = YES;
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:nil];	
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)setNavigationController:(UINavigationController *)navigationController {
	if (self.navigationController != nil) {
		[_navigationController release];
	}
	
	_navigationController = [navigationController retain];
	self.tableViewController.navigationController = self.navigationController;	
}

- (void)setEditing:(BOOL)editing animated:(BOOL)animated {
	[self.tableViewController toggleEditing];
	
	[super setEditing:editing animated:animated];
}

#pragma mark -
#pragma mark UI Actions
- (void)add:(id)sender {
	SymbolAddController *addController = [[SymbolAddController alloc] initWithNibName:@"SymbolAddView" bundle:nil];
	addController.delegate = self;
	addController.managedObjectContext = self.managedObjectContext;
	
	UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:addController];
	navigationController.navigationBar.tintColor = [UIColor colorWithRed:0.33f green:0.78f blue:0.07f alpha:1.0f];
	[addController release];
	[self.parentViewController presentModalViewController:navigationController animated:YES];
	[navigationController release];
}

- (void)symbolAddControllerDidFinish:(SymbolAddController *)stockSearchController didAddSymbol:(NSString *)tickerSymbol {
	[self dismissModalViewControllerAnimated:YES];
}

//#pragma mark -
//#pragma mark Debugging methods
//// Very helpful debug when things seem not to be working.
//- (BOOL)respondsToSelector:(SEL)sel {
//	NSLog(@"Queried about %@ in MyListViewController", NSStringFromSelector(sel));
//	return [super respondsToSelector:sel];
//}


#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_managedObjectContext release];
	[_tableViewController release];
	
    [super dealloc];
}


@end
