//
//  NewsController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NewsController.h"

#import "mTraderAppDelegate.h"
#import "UserDefaults.h"
#import "FeedsTableViewController.h"

#import "NewsFeed.h"
#import "NewsArticle.h"
#import "Feed.h"
#import "Symbol.h"
#import "NewsCell.h"
#import "NewsArticleController.h"
#import "SymbolDataController.h"
#import "QFields.h"

@implementation NewsController
@synthesize communicator;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize feedsFetchedResultsController = _feedsFetchedResultsController;
#ifdef UI_USER_INTERFACE_IDIOM
@synthesize feedsPopover = _feedsPopover;
#endif
@synthesize newsFeed = _newsFeed;

#pragma mark -
#pragma mark Initialization

- (id)initWithMangagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
    if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		_fetchedResultsController = nil;
		_feedsFetchedResultsController = nil;
#ifdef UI_USER_INTERFACE_IDIOM
		_feedsPopover = nil;
#endif
		_newsFeed = nil;
		
		UIImage* anImage = [UIImage imageNamed:@"NewsTab.png"];	
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"NewsTab", "News tab label")  image:anImage tag:NEWS];
		self.tabBarItem = theItem;
		[theItem release];
	}
    return self;
}

- (void)viewDidLoad {		
	// Set the feed to All News by default
	NSString *returnedNewsFeedNumber = [UserDefaults sharedManager].newsFeedNumber;
	NSString *feedNumber = nil;
	
	NSInteger feedIntegerValue = [returnedNewsFeedNumber integerValue];
	if (returnedNewsFeedNumber && feedIntegerValue >= 0) {
		feedNumber = returnedNewsFeedNumber;
	} else {
		feedNumber = @"0";
		[UserDefaults sharedManager].newsFeedNumber = feedNumber;
	}
		
	self.newsFeed = [[SymbolDataController sharedManager] fetchNewsFeedWithNumber:feedNumber];
	self.title = self.newsFeed.name;
	self.tabBarItem.title = NSLocalizedString(@"NewsTab", "News tab label");
	
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}

	if (![self.feedsFetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
	
	feedBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Feeds" style:UIBarButtonItemStyleBordered target:self action:@selector(feedBarButtonItemAction:)];
	self.navigationItem.leftBarButtonItem = feedBarButtonItem;
	[feedBarButtonItem release];
		
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
	
	feedsTableViewController = [[FeedsTableViewController alloc] init];
	feedsTableViewController.delegate = self;
	feedsTableViewController.managedObjectContext = self.managedObjectContext;
	
#ifdef UI_USER_INTERFACE_IDIOM
	_feedsPopover = [[UIPopoverController alloc] initWithContentViewController:feedsTableViewController];
#endif
	[feedsTableViewController release];
	
	[super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {	
	self.tableView.frame = self.view.bounds;
	
	communicator = [mTraderCommunicator sharedManager];
	
	QFields *qFields = [[QFields alloc] init];
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:nil];
	
	[communicator newsListFeed:self.newsFeed.mCode];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

#pragma mark -
#pragma mark TableView datasource methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [[self.fetchedResultsController sections] count];
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:section];
    return [sectionInfo numberOfObjects];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	NSInteger count = [[self.fetchedResultsController sections] count];
	if (count > 0) {
		id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:section];
		return [sectionInfo name];
	}
	return nil;	
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"NewsCell";
    
    NewsCell *cell = (NewsCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[NewsCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(NewsCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	NewsArticle *newsArticle = (NewsArticle *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	cell.newsArticle = newsArticle;
}

#pragma mark -
#pragma mark TableView delegate methods

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NewsArticleController *newsArticleController = [[NewsArticleController alloc] init];

	NewsArticle *newsArticle = (NewsArticle *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	newsArticleController.newsArticle = newsArticle;
	
	[self.navigationController pushViewController:newsArticleController animated:YES];
	
	[newsArticleController release];
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}


#pragma mark -
#pragma mark UIPickerViewDataSource Required Methods
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
	return [[self.feedsFetchedResultsController sections] count];
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
	id <NSFetchedResultsSectionInfo> sectionInfo = [[self.feedsFetchedResultsController sections] objectAtIndex:component];
    return [sectionInfo numberOfObjects];
}

#pragma mark -
#pragma mark UIPickerViewDelegate Methods
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	self.newsFeed = (NewsFeed *)[self.feedsFetchedResultsController objectAtIndexPath:indexPath];
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	NewsFeed *feed = (NewsFeed *)[self.feedsFetchedResultsController objectAtIndexPath:indexPath];
	NSString *feedName = feed.name;
	return feedName;
}

#pragma mark -
#pragma mark Actions

- (void)refresh:(id)sender {
	[self.communicator newsListFeed:self.newsFeed.mCode];
}

- (void)feedBarButtonItemAction:(id)sender {
	BOOL iPad = NO;
#ifdef UI_USER_INTERFACE_IDIOM
	iPad = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
#endif
	
	if (iPad) {
#ifdef UI_USER_INTERFACE_IDIOM
		if ([_feedsPopover isPopoverVisible]) {
			[_feedsPopover dismissPopoverAnimated:YES];
		} else {
			[_feedsPopover presentPopoverFromBarButtonItem:feedBarButtonItem permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
		}
#endif
	} else {
		FeedsTableViewController *feedsModalTableViewController = [[FeedsTableViewController alloc] init];
		feedsModalTableViewController.delegate = self;
		feedsModalTableViewController.managedObjectContext = self.managedObjectContext;
		feedsModalTableViewController.title = @"Select News Feed";

		UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:feedsModalTableViewController];
				
		UIBarButtonItem *doneBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone	target:self action:@selector(doneBarButtonItemAction:)];
		feedsModalTableViewController.navigationItem.leftBarButtonItem = doneBarButtonItem;
		[doneBarButtonItem release];
		[feedsModalTableViewController release];
		
		[self presentModalViewController:navController animated:YES];
		[navController release];
	}
}

- (void)doneBarButtonItemAction:(id)sender {
	[self dismissModalViewControllerAnimated:YES];
}

#pragma mark -
#pragma mark NewsFeedChoiceDelegate methods
- (void)newsFeedWasSelected:(NewsFeed *)aNewsFeed {
	// Dismiss the popover by calling the toggle
	[self feedBarButtonItemAction:nil];
	[[SymbolDataController sharedManager] deleteAllNews];
	
	// Fire off request for news related to the choice made if different from current choice
	if ([aNewsFeed.feedNumber isEqualToString:self.newsFeed.feedNumber]) {
		return;
	} else {
		self.newsFeed = aNewsFeed;
		self.title = aNewsFeed.name;
		self.tabBarItem.title = NSLocalizedString(@"NewsTab", "News tab label");
		[[mTraderCommunicator sharedManager] newsListFeed:aNewsFeed.mCode];	
		[UserDefaults sharedManager].newsFeedNumber = aNewsFeed.feedNumber;
	}
}

#pragma mark -
#pragma mark Fetched results controller

/**
 Returns the fetched results controller. Creates and configures the controller if necessary.
 */
- (NSFetchedResultsController *)fetchedResultsController {
	
    if (_fetchedResultsController != nil) {
        return _fetchedResultsController;
    }
    
	// Create and configure a fetch request with the Book entity.
	NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"NewsArticle" inManagedObjectContext:self.managedObjectContext];
	[fetchRequest setEntity:entity];
	
	// Create the sort descriptors array.
	NSSortDescriptor *dateDescriptor = [[NSSortDescriptor alloc] initWithKey:@"date" ascending:NO];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:dateDescriptor, nil];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
	self.fetchedResultsController = aFetchedResultsController;
	_fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[dateDescriptor release];
	[sortDescriptors release];
	
	return _fetchedResultsController;
}

- (NSFetchedResultsController *)feedsFetchedResultsController {
	
    if (_feedsFetchedResultsController != nil) {
        return _feedsFetchedResultsController;
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
	self.feedsFetchedResultsController = aFetchedResultsController;
	_feedsFetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[mCodeDescriptor release];
	[sortDescriptors release];
	
	return _feedsFetchedResultsController;
}


/**
 Delegate methods of NSFetchedResultsController to respond to additions, removals and so on.
 */

- (void)controllerWillChangeContent:(NSFetchedResultsController *)controller {
	
	// The fetch controller is about to start sending change notifications, so prepare the table view for updates.
	[self.tableView beginUpdates];
}


- (void)controller:(NSFetchedResultsController *)controller didChangeObject:(id)anObject atIndexPath:(NSIndexPath *)indexPath forChangeType:(NSFetchedResultsChangeType)type newIndexPath:(NSIndexPath *)newIndexPath {
	UITableView *tableView = self.tableView;

	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationFade];
			break;
			
		case NSFetchedResultsChangeDelete:
			[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
			break;
    }
}


- (void)controller:(NSFetchedResultsController *)controller didChangeSection:(id <NSFetchedResultsSectionInfo>)sectionInfo atIndex:(NSUInteger)sectionIndex forChangeType:(NSFetchedResultsChangeType)type {
	
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[self.tableView insertSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationFade];
			break;
			
		case NSFetchedResultsChangeDelete:
			[self.tableView deleteSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationFade];
			break;
	}
}


- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
	// The fetch controller has sent all current change notifications, so tell the table view to process all updates.
	[self.tableView endUpdates];
}

/*
#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in NewsController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
*/
#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
#ifdef UI_USER_INTERFACE_IDIOM
	self.feedsPopover = nil;
#endif
	self.newsFeed = nil;
}

- (void)dealloc {
#ifdef UI_USER_INTERFACE_IDIOM
	[_feedsPopover release];
#endif	
	[_fetchedResultsController release];
	[_feedsFetchedResultsController release];

	[_managedObjectContext release];
	
	[_newsFeed release];
    [super dealloc];
}


@end
