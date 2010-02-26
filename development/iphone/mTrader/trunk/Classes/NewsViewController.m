//
//  NewsViewController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "NewsViewController.h"
#import "NewsItemViewController.h"
#import "mTraderAppDelegate.h"
#import "mTraderCommunicator.h"
#import "NewsFeed.h"

@implementation NewsViewController
@synthesize managedObjectContext, fetchedResultsController;
@synthesize communicator;
@synthesize newsArray = _newsArray;

#pragma mark Lifecycle

- (id)init {
	self = [super init];
	if (self != nil) {
		self.title = NSLocalizedString(@"NewsTab", "News tab label");
		UIImage* anImage = [UIImage imageNamed:@"newsTabButton.png"];	
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"NewsTab", "News tab label")  image:anImage tag:NEWS];
		self.tabBarItem = theItem;
		[theItem release];
		_newsArray = [[NSMutableArray alloc] init];

		communicator = [mTraderCommunicator sharedManager];
		mCode = @"AllNews";
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


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	[super loadView];
	
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	UIBarButtonItem *selectFeed = [[UIBarButtonItem alloc] initWithTitle:@"Feeds" style:UIBarButtonItemStyleBordered target:self action:@selector(selectNewsFeed:)];
	self.navigationItem.leftBarButtonItem = selectFeed;
	[selectFeed release];
	
	UIBarButtonItem *refreshNews = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refreshNews)];
	self.navigationItem.rightBarButtonItem = refreshNews;
	[refreshNews release];
}

- (void)viewWillAppear:(BOOL)animated {
	self.communicator.symbolsDelegate = self;
	[self.communicator newsListFeed:mCode];
	[self.communicator stopStreamingData];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait);
	return NO;
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


#pragma mark TableViewDataSourceDelegate Methods
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [self.newsArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	static NSString *CellIdentifier = @"NewsCell";
	
	UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	if (cell == nil) {
		cell = [[UITableViewCell alloc] init];
		cell.textLabel.numberOfLines = 0;
	}
	
	NSString *cellText = [[self.newsArray objectAtIndex:indexPath.row] objectAtIndex:1];
	if ([cellText rangeOfString:@"***"].location == 0) {
		[cell.contentView setBackgroundColor:[UIColor redColor]];
		cellText = [cellText substringFromIndex:3];
	} else if ([cellText rangeOfString:@"*"].location == 0) {
		[cell.contentView setBackgroundColor:[UIColor blueColor]];
		cellText = [cellText substringFromIndex:1];
	} else if ([cellText rangeOfString:@"="].location == 0) {
		[cell.contentView setBackgroundColor:[UIColor greenColor]];
		cellText = [cellText substringFromIndex:1];
	}
	UIFont *font = [UIFont fontWithName:@"Courier" size:14];
	[cell.textLabel setFont:font];
	[cell.textLabel setText:cellText];
	
	return cell;
}
#pragma mark -
#pragma mark UIPickerViewDataSource Required Methods
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
	return [[fetchedResultsController sections] count];
}


- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
	id <NSFetchedResultsSectionInfo> sectionInfo = [[fetchedResultsController sections] objectAtIndex:component];
    return [sectionInfo numberOfObjects];
}

#pragma mark TableViewDelegate Methods
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NSUInteger row = indexPath.row;
	
	NSArray *tuple = [self.newsArray objectAtIndex:row];
	NSString *newsID = [tuple objectAtIndex:0];
	NewsItemViewController *newsItemViewController = [[NewsItemViewController alloc] initWithNewsItem:newsID];
	[self.navigationController pushViewController:newsItemViewController animated:YES];
	
}

#pragma mark -
#pragma mark UIPickerViewDelegate Methods
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	NewsFeed *feed = (NewsFeed *)[fetchedResultsController objectAtIndexPath:indexPath];
	mCode = feed.mCode;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	NewsFeed *feed = (NewsFeed *)[fetchedResultsController objectAtIndexPath:indexPath];
	NSString *feedName = feed.name;
	return feedName;
}

#pragma mark Delegation

-(void) newsListFeedsUpdates:(NSArray *)newsList {
	for (NSString *news in newsList) {
		NSArray *components = [news componentsSeparatedByString:@";"];
		if ([components count] >= 4) {
			NSString *key = [components objectAtIndex:0];
			NSString *value = [components objectAtIndex:4];
			NSArray *tuple = [NSArray arrayWithObjects:key, value, nil];
		
			[self.newsArray addObject:tuple];
		}
	}
	
	[self.tableView reloadData];
}


#pragma mark News Refresh

-(void) refreshNews {
	[self.newsArray removeAllObjects];
	[self.communicator newsListFeed:mCode];
	[self.tableView reloadData];
}

- (void)selectNewsFeed:(id)sender {
	UIActionSheet *menu = [[UIActionSheet alloc] initWithTitle:@"Select a News Feed" delegate:self cancelButtonTitle:@"Done" destructiveButtonTitle:@"Cancel" otherButtonTitles:@"All News Feeds", nil];
	UIPickerView *feedSelector = [[UIPickerView alloc] initWithFrame:CGRectMake(0.0, 218.0, 0.0, 0.0)];
	feedSelector.showsSelectionIndicator = YES;
	feedSelector.dataSource = self;
	feedSelector.delegate = self;
	[menu addSubview:feedSelector];
	[feedSelector release];
	[menu showInView:self.tableView];
	[menu setBounds:CGRectMake(0.0, 0.0, 320.0, 700.0)];
	[menu release];
}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
	if (buttonIndex == 0) {
		return;
	} else if (buttonIndex == 1) {
		mCode = @"AllNews";
		[self.newsArray removeAllObjects];
		[communicator newsListFeed:mCode];
	} else if (buttonIndex == 2) {
		// Selected News Feed
		[self.newsArray removeAllObjects];
		[communicator newsListFeed:mCode];
	}
}

#pragma mark -
#pragma mark Fetched results controller

/**
 Returns the fetched results controller. Creates and configures the controller if necessary.
 */
- (NSFetchedResultsController *)fetchedResultsController {
	
    if (fetchedResultsController != nil) {
        return fetchedResultsController;
    }
    
	// Create and configure a fetch request with the Book entity.
	NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"NewsFeed" inManagedObjectContext:self.managedObjectContext];
	[fetchRequest setEntity:entity];
	
	// Create the sort descriptors array.
	NSSortDescriptor *mCodeDescriptor = [[NSSortDescriptor alloc] initWithKey:@"name" ascending:YES];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:mCodeDescriptor, nil];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
	self.fetchedResultsController = aFetchedResultsController;
	fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[mCodeDescriptor release];
	[sortDescriptors release];
	
	return fetchedResultsController;
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[self.newsArray release];
    [super dealloc];
}

@end
