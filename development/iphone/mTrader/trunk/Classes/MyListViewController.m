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

- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
	if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		_frame = frame;
		_tableViewController = [[ChainsTableViewController alloc] initWithManagedObjectContext:self.managedObjectContext];
	}
	return self;
}

#define TEXT_LEFT_MARGIN 8.0
#define TEXT_RIGHT_MARGIN 8.0

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	UIView *aView = [[UIView alloc] initWithFrame:_frame];
	aView.autoresizesSubviews = YES;
	self.view = aView;
	[aView release];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	self.title = NSLocalizedString(@"ChainsTab", @"Chains tab label");
	
	CGRect frame = self.view.bounds;
	
	self.view.backgroundColor = [UIColor whiteColor];
	
	UIImage *mTraderImage = [UIImage imageNamed:@"Mtrader_16.png"];
	UIImageView *mTraderBranding = [[UIImageView alloc] initWithFrame:CGRectMake(TEXT_LEFT_MARGIN, 10.0f, 112.0f, 16.0f)];
	mTraderBranding.image = mTraderImage;
	[self.view addSubview:mTraderBranding];
	
	CGRect buttonFrame = CGRectMake(0.0f, 2.0f, 85.0f, 37.0f);
	CGRect buttonBounds = CGRectMake(0.0f, 0.0f, 83.0f, 37.0f);
	
	BOOL iPad = NO;
	#ifdef UI_USER_INTERFACE_IDIOM
	iPad = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
	#endif
	
	
	if (iPad) {
		buttonFrame.origin.x = frame.size.width - 85.0f * 5.0f - TEXT_RIGHT_MARGIN;
		UIButton *bidButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		[bidButton setTitle:@"Bid" forState:UIControlStateNormal];
		bidButton.frame = buttonFrame;
		bidButton.bounds = buttonBounds;
		bidButton.backgroundColor = [UIColor whiteColor];
	
		buttonFrame.origin.x = frame.size.width - 85.0f * 4.0f - TEXT_RIGHT_MARGIN;
		UIButton *askButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		[askButton setTitle:@"Ask" forState:UIControlStateNormal];
		askButton.frame = buttonFrame;
		askButton.bounds = buttonBounds;
		askButton.backgroundColor = [UIColor whiteColor];
	
		unichar upDownArrowsChar = 0x21C5;
		NSString *upDownArrows = [NSString stringWithCharacters:&upDownArrowsChar length:1];
		buttonFrame.origin.x = frame.size.width - 85.0f * 2.0f - TEXT_RIGHT_MARGIN;
		UIButton *upDownButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		[upDownButton setTitle:upDownArrows forState:UIControlStateNormal];
		upDownButton.frame = buttonFrame;
		upDownButton.bounds = buttonBounds;
		upDownButton.backgroundColor = [UIColor whiteColor];
		
		[self.view addSubview:bidButton];
		[self.view addSubview:askButton];
		[self.view addSubview:upDownButton];
		
		buttonFrame.origin.x = frame.size.width - 85.0f * 3.0f - TEXT_RIGHT_MARGIN;
	} else {
		buttonFrame.origin.x = frame.size.width - 85.0f * 2.0f - TEXT_RIGHT_MARGIN;
	}
	
	UIButton *centerButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[centerButton setTitle:@"Last" forState:UIControlStateNormal];
	centerButton.frame = buttonFrame;
	centerButton.bounds = buttonBounds;
	centerButton.backgroundColor = [UIColor whiteColor];
	
	buttonFrame.origin.x = frame.size.width - 85.0f - TEXT_RIGHT_MARGIN;
	UIButton *rightButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[rightButton setTitle:@"%" forState:UIControlStateNormal];
	rightButton.frame = buttonFrame;
	rightButton.bounds = buttonBounds;
	rightButton.backgroundColor = [UIColor whiteColor];
	
	if (!iPad) {
		[centerButton addTarget:self.tableViewController action:@selector(centerSelection:) forControlEvents:UIControlEventTouchUpInside];
		[rightButton addTarget:self.tableViewController action:@selector(rightSelection:) forControlEvents:UIControlEventTouchUpInside];
	}
	
	frame.origin.y = 39.0f;
	frame.size.height -= 39.0f;
	self.tableViewController.view.frame = frame;
	
	[self.view addSubview:centerButton];
	[self.view addSubview:rightButton];
	[self.view addSubview:self.tableViewController.view];

	//CGRect buttonFrame = CGRectMake(0.0f, 0.0f, 85.0f, 37.0f);
	self.navigationItem.leftBarButtonItem = self.editButtonItem;	
	
	// Setup right and left bar buttons
	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(add:)];
	self.navigationItem.rightBarButtonItem = addItem;
	[addItem release];
}

- (void)viewWillAppear:(BOOL)animated {
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

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

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
	SymbolAddController *controller = [[SymbolAddController alloc] init];
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
