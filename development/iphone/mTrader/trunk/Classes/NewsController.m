//
//  NewsController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "NewsController.h"

#import "mTraderAppDelegate.h"
#import "NewsFeed.h"
#import "NewsArticle.h"
#import "Feed.h"
#import "Symbol.h"
#import "NewsCell.h"
#import "NewsArticleController.h"
#import "SymbolDataController.h"

@implementation NewsController
@synthesize communicator;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize feedsFetchedResultsController = _feedsFetchedResultsController;
@synthesize mCode = _mCode;

#pragma mark -
#pragma mark Initialization

- (id)initWithMangagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
    if (self != nil) {
		self.managedObjectContext = managedObjectContext;
		_fetchedResultsController = nil;
		_feedsFetchedResultsController = nil;
		
		self.title = NSLocalizedString(@"NewsTab", "News tab label");
		UIImage* anImage = [UIImage imageNamed:@"newsTabButton.png"];	
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"NewsTab", "News tab label")  image:anImage tag:NEWS];
		self.tabBarItem = theItem;
		[theItem release];
		
		table = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
		table.delegate = self;
		table.dataSource = self;
		
		[self.view addSubview:table];
		
		self.mCode = @"AllNews";
	}
    return self;
}

- (void)viewWillAppear:(BOOL)animated {	
	table.frame = self.view.bounds;
	
	communicator = [mTraderCommunicator sharedManager];
	[communicator newsListFeed:self.mCode];
}

- (void)viewDidLoad {
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
	
	UIBarButtonItem *selectFeed = [[UIBarButtonItem alloc] initWithTitle:@"Feeds" style:UIBarButtonItemStyleBordered target:self action:@selector(selectNewsFeed:)];
	self.navigationItem.leftBarButtonItem = selectFeed;
	[selectFeed release];
		
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
		
	[super viewDidLoad];
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
	NewsFeed *feed = (NewsFeed *)[self.feedsFetchedResultsController objectAtIndexPath:indexPath];
	self.mCode = feed.mCode;
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
	[[SymbolDataController sharedManager] deleteAllNews];
	[self.communicator newsListFeed:self.mCode];
}

- (void)selectNewsFeed:(id)sender {
	UIActionSheet *menu = [[UIActionSheet alloc] initWithTitle:@"Select a News Feed" delegate:self cancelButtonTitle:@"Done" destructiveButtonTitle:@"Cancel" otherButtonTitles:@"All News Feeds", nil];
	UIPickerView *feedSelector = [[UIPickerView alloc] initWithFrame:CGRectMake(0.0, 218.0, 0.0, 0.0)];
	feedSelector.showsSelectionIndicator = YES;
	feedSelector.dataSource = self;
	feedSelector.delegate = self;
	[menu addSubview:feedSelector];
	[feedSelector release];
	[menu showInView:table];
	[menu setBounds:CGRectMake(0.0, 0.0, 320.0, 700.0)];
	[menu release];
}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
	if (buttonIndex == 0) {
		return;
	} else if (buttonIndex == 1) {
		self.mCode = @"AllNews";
		[communicator newsListFeed:self.mCode];
	} else if (buttonIndex == 2) {
		[communicator newsListFeed:self.mCode];
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
	NSSortDescriptor *mCodeDescriptor = [[NSSortDescriptor alloc] initWithKey:@"articleNumber" ascending:YES];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:mCodeDescriptor, nil];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
	self.fetchedResultsController = aFetchedResultsController;
	_fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[mCodeDescriptor release];
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
	[table beginUpdates];
}


- (void)controller:(NSFetchedResultsController *)controller didChangeObject:(id)anObject atIndexPath:(NSIndexPath *)indexPath forChangeType:(NSFetchedResultsChangeType)type newIndexPath:(NSIndexPath *)newIndexPath {
	
	UITableView *tableView = table;
	
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationFade];
			break;
			
		case NSFetchedResultsChangeDelete:
			[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
			break;
			
		case NSFetchedResultsChangeUpdate:
			[self configureCell:(NewsCell *)[tableView cellForRowAtIndexPath:indexPath] atIndexPath:indexPath animated:YES];
			break;
			
		case NSFetchedResultsChangeMove:
            [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
            [tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationFade];
            break;
    }
}


- (void)controller:(NSFetchedResultsController *)controller didChangeSection:(id <NSFetchedResultsSectionInfo>)sectionInfo atIndex:(NSUInteger)sectionIndex forChangeType:(NSFetchedResultsChangeType)type {
	
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[table insertSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationFade];
			break;
			
		case NSFetchedResultsChangeDelete:
			[table deleteSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationFade];
			break;
	}
}


- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
	// The fetch controller has sent all current change notifications, so tell the table view to process all updates.
	[table endUpdates];
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

- (void)dealloc {
	[_fetchedResultsController release];
	[_feedsFetchedResultsController release];

	[_managedObjectContext release];
	
	[_mCode release];
	[table release];
    [super dealloc];
}


@end
