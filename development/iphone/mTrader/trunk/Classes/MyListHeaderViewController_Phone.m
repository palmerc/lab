//
//  MyListViewController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "MyListHeaderViewController_Phone.h"

#import "MyListTableViewController_Phone.h"
#import "QFields.h"

@implementation MyListHeaderViewController_Phone
@synthesize managedObjectContext = _managedObjectContext;
@synthesize tableViewController = _tableViewController;
@synthesize navigationController = _navigationController; 

- (id)initWithFrame:(CGRect)frame {
	self = [super init];
	if (self != nil) {
		self.title = NSLocalizedString(@"MyList", @"My List tab label");

		_frame = frame;
		_tableViewController = nil;
		_navigationController = nil;
	}
	return self;
}

#define TEXT_LEFT_MARGIN 8.0
#define TEXT_RIGHT_MARGIN 8.0

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	UIView *aView = [[UIView alloc] initWithFrame:_frame];
	aView.autoresizesSubviews = YES;
	
	CGRect bounds = aView.bounds;
	
	aView.backgroundColor = [UIColor whiteColor];
	
	UIImage *mTraderImage = [UIImage imageNamed:@"Mtrader_16.png"];
	CGRect mTraderImageFrame = CGRectMake(TEXT_LEFT_MARGIN, 10.0f, mTraderImage.size.width, mTraderImage.size.height);
	UIImageView *mTraderBranding = [[UIImageView alloc] initWithFrame:mTraderImageFrame];
	mTraderBranding.image = mTraderImage;
	[aView addSubview:mTraderBranding];
	[mTraderBranding release];
	
	CGRect buttonFrame = CGRectMake(0.0f, 2.0f, 85.0f, 37.0f);
	CGRect buttonBounds = CGRectMake(0.0f, 0.0f, 83.0f, 37.0f);
	
	buttonFrame.origin.x = bounds.size.width - 85.0f * 2.0f - TEXT_RIGHT_MARGIN;
	
	UIButton *centerButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[centerButton setTitle:@"Last" forState:UIControlStateNormal];
	centerButton.frame = buttonFrame;
	centerButton.bounds = buttonBounds;
	centerButton.backgroundColor = [UIColor whiteColor];
	
	buttonFrame.origin.x = bounds.size.width - 85.0f - TEXT_RIGHT_MARGIN;
	UIButton *rightButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[rightButton setTitle:@"%" forState:UIControlStateNormal];
	rightButton.frame = buttonFrame;
	rightButton.bounds = buttonBounds;
	rightButton.backgroundColor = [UIColor whiteColor];
	
	_tableViewController = [[MyListTableViewController_Phone alloc] initWithFrame:aView.bounds];
	_tableViewController.navigationController = self.navigationController;
	[centerButton addTarget:self.tableViewController action:@selector(centerSelection:) forControlEvents:UIControlEventTouchUpInside];
	[rightButton addTarget:self.tableViewController action:@selector(rightSelection:) forControlEvents:UIControlEventTouchUpInside];
	
	bounds.origin.y = 39.0f;
	bounds.size.height -= 39.0f;
	self.tableViewController.view.frame = bounds;
	
	[aView addSubview:centerButton];
	[aView addSubview:rightButton];
	[aView addSubview:self.tableViewController.view];
	
	self.navigationItem.leftBarButtonItem = self.editButtonItem;	
	
	// Setup right and left bar buttons
	UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(add:)];
	self.navigationItem.rightBarButtonItem = addItem;
	[addItem release];
	
	self.view = aView;
	[aView release];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	self.tableViewController.managedObjectContext = self.managedObjectContext;
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

- (void)setEditing:(BOOL)editing animated:(BOOL)animated {
	[self.tableViewController toggleEditing];
	
	[super setEditing:editing animated:animated];
}

#pragma mark -
#pragma mark UI Actions
- (void)add:(id)sender {
	CGRect windowFrame = [[UIScreen mainScreen] applicationFrame];
	SymbolAddController_Phone *symbolAddController = [[SymbolAddController_Phone alloc] initWithFrame:windowFrame];
	symbolAddController.delegate = self;
	
	UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:symbolAddController];
	[symbolAddController release];
	[self.parentViewController presentModalViewController:navigationController animated:YES];
	[navigationController release];
}

- (void)symbolAddControllerDidFinish:(SymbolAddController_Phone *)stockSearchController didAddSymbol:(NSString *)tickerSymbol {
	[self dismissModalViewControllerAnimated:YES];
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
#if DEBUG
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in MyListHeaderViewController_Phone", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif


#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_managedObjectContext release];
	[_tableViewController release];
	[_navigationController release];
    [super dealloc];
}


@end
