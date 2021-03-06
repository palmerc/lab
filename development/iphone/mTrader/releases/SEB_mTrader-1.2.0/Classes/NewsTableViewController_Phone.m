//
//  NewsTableViewController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "NewsTableViewController_Phone.h"

#import "mTraderAppDelegate_Phone.h"
#import "UserDefaults.h"
#import "FeedsTableViewController_Phone.h"

#import "NewsFeed.h"
#import "NewsArticle.h"
#import "Feed.h"
#import "Symbol.h"
#import "NewsTableViewCell_Phone.h"
#import "NewsArticleController_Phone.h"
#import "DataController.h"
#import "QFields.h"

@implementation NewsTableViewContoller_Phone
@synthesize communicator;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize feedsFetchedResultsController = _feedsFetchedResultsController;
@synthesize newsFeed = _newsFeed;

#pragma mark -
#pragma mark Initialization

- (id)initWithFrame:(CGRect)frame {
	self = [super init];
    if (self != nil) {
		_frame = frame;
		_managedObjectContext = nil;
		_fetchedResultsController = nil;
		_feedsFetchedResultsController = nil;
		_newsFeed = nil;
		_headlineFont = [UIFont systemFontOfSize:14.0f];
		_bottomlineFont = [UIFont systemFontOfSize:12.0f];
		
		UIImage* anImage = [UIImage imageNamed:@"NewsTab.png"];	
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"NewsTab", "News tab label")  image:anImage tag:NEWS];
		self.tabBarItem = theItem;
		[theItem release];
	}
    return self;
}

- (void)viewDidLoad {		
	[super viewDidLoad];
	
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
		
	self.newsFeed = [[DataController sharedManager] fetchNewsFeedWithNumber:feedNumber];
	self.title = self.newsFeed.name;
	self.tabBarItem.title = NSLocalizedString(@"NewsTab", "News tab label");
	
	self.navigationItem.backBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Back" style: UIBarButtonItemStyleBordered target:nil action:nil];
	
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}

	if (![self.feedsFetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();  // Fail
#endif
	}
	
	feedBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Feeds" style:UIBarButtonItemStyleBordered target:self action:@selector(feedBarButtonItemAction:)];
	self.navigationItem.leftBarButtonItem = feedBarButtonItem;
	[feedBarButtonItem release];
		
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
	
	[communicator newsListFeed:self.newsFeed.mCode];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
		
	communicator = [mTraderCommunicator sharedManager];
	
	QFields *qFields = [[QFields alloc] init];
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:nil];
	
	[communicator newsListFeed:self.newsFeed.mCode];
}

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

#define CONTENT_MARGIN 10.0f
// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"NewsCell";
    
    NewsTableViewCell_Phone *cell = (NewsTableViewCell_Phone *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[NewsTableViewCell_Phone alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		cell.headlineFont = _headlineFont;
		cell.bottomlineFont = _bottomlineFont;
		cell.contentMargin = CONTENT_MARGIN;
    }
    
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(NewsTableViewCell_Phone *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	NewsArticle *newsArticle = (NewsArticle *)[self.fetchedResultsController objectAtIndexPath:indexPath];

	cell.newsArticle = newsArticle;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	NewsArticle *newsArticle = (NewsArticle *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	NSString *headlineString = newsArticle.headline;
	NSString *bottomLineString = @"Infront News";
	
	CGFloat windowWidth = self.view.frame.size.width;
	CGSize constraint = CGSizeMake(windowWidth - (CONTENT_MARGIN * 2.0f), 2000.0f);
	CGSize headlineSize = [headlineString sizeWithFont:_headlineFont constrainedToSize:constraint lineBreakMode:UILineBreakModeWordWrap];
	CGSize bottomlineSize = [bottomLineString sizeWithFont:_bottomlineFont];
	
	CGFloat height = headlineSize.height + bottomlineSize.height + CONTENT_MARGIN;
	return height;
}

#pragma mark -
#pragma mark TableView delegate methods

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NewsArticleController_Phone *newsArticleController = [[NewsArticleController_Phone alloc] init];

	NewsArticle *newsArticle = (NewsArticle *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	newsArticleController.newsArticle = newsArticle;
	
	[self.navigationController pushViewController:newsArticleController animated:YES];
	
	[newsArticleController release];
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

#pragma mark -
#pragma mark Actions

- (void)refresh:(id)sender {
	[self.communicator newsListFeed:self.newsFeed.mCode];
}

- (void)feedBarButtonItemAction:(id)sender {
	FeedsTableViewController_Phone *feedsModalTableViewController = [[FeedsTableViewController_Phone alloc] init];
	feedsModalTableViewController.delegate = self;
	feedsModalTableViewController.managedObjectContext = self.managedObjectContext;
	
	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:feedsModalTableViewController];
	
	[feedsModalTableViewController release];
	
	[self presentModalViewController:navController animated:YES];
	[navController release];
}

#pragma mark -
#pragma mark NewsFeedChoiceDelegate methods
- (void)newsFeedWasSelected:(NewsFeed *)aNewsFeed {
	[self dismissModalViewControllerAnimated:YES];
	
	// Fire off request for news related to the choice made if different from current choice
	if ([aNewsFeed.feedNumber isEqualToString:self.newsFeed.feedNumber]) {
		return;
	} else {
		self.fetchedResultsController = nil;		
		self.newsFeed = aNewsFeed;
		self.title = aNewsFeed.name;
		self.tabBarItem.title = NSLocalizedString(@"NewsTab", "News tab label");
		[[mTraderCommunicator sharedManager] newsListFeed:aNewsFeed.mCode];	
		[UserDefaults sharedManager].newsFeedNumber = aNewsFeed.feedNumber;
		
		NSError *error;
		if (![self.fetchedResultsController performFetch:&error]) {
			// Update to handle the error appropriately.
			NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
			abort();  // Fail
#endif
		}
		
		[self.tableView reloadData];
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
	
	// Special Case the All News
	if (![self.newsFeed.feedNumber isEqualToString:@"0"]) {
		NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(newsFeed.feedNumber=%@)", self.newsFeed.feedNumber];
		[fetchRequest setPredicate:predicate];	
	}
	
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


#pragma mark -
#pragma mark Debugging methods

#if DEBUG
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in NewsController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

#pragma mark -
#pragma mark Memory management

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	self.newsFeed = nil;
}

- (void)dealloc {
	[_fetchedResultsController release];
	[_feedsFetchedResultsController release];

	[_managedObjectContext release];
	
	[_newsFeed release];
    [super dealloc];
}


@end
