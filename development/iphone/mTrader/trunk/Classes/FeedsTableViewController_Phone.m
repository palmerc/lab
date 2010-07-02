//
//  FeedsTableViewController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 04.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "FeedsTableViewController_Phone.h"

#import "NewsFeed.h"
#import "UserDefaults.h"

@implementation FeedsTableViewController_Phone
@synthesize delegate;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize selectedNewsFeed = _selectedNewsFeed;

#pragma mark -
#pragma mark Initialization

#pragma mark -
#pragma mark UIViewController overridden methods

- (void)viewDidLoad {
    [super viewDidLoad];
	
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
	
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
	self.fetchedResultsController = nil;
}

#pragma mark -
#pragma mark UITableViewController data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [[self.fetchedResultsController sections] count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:section];
    return [sectionInfo numberOfObjects];
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	NewsFeed *feed = (NewsFeed *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	NSString *feedName = feed.name;
    
	[cell.textLabel setText:feedName];
	
	NSString *currentNumber = [UserDefaults sharedManager].newsFeedNumber;
	
	if ([feed.feedNumber isEqualToString:currentNumber]) {
		cell.accessoryType = UITableViewCellAccessoryCheckmark;
		self.selectedNewsFeed = feed;
	} else {
		cell.accessoryType = UITableViewCellAccessoryNone;
	}

	return cell;
}

#pragma mark -
#pragma mark UITableViewController delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	[tableView deselectRowAtIndexPath:indexPath animated:NO];
	NSIndexPath *oldIndexPath = [self.fetchedResultsController indexPathForObject:self.selectedNewsFeed];
	
	if ([indexPath isEqual:oldIndexPath]) {
		return;
	}
	
	NewsFeed *newsFeed = (NewsFeed *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	self.selectedNewsFeed = newsFeed;
	
	[UserDefaults sharedManager].newsFeedNumber = newsFeed.feedNumber;
	
	UITableViewCell *newCell = [tableView cellForRowAtIndexPath:indexPath];
	if (newCell.accessoryType == UITableViewCellAccessoryNone) {
		newCell.accessoryType = UITableViewCellAccessoryCheckmark;
	}
	
	UITableViewCell *oldCell = [tableView cellForRowAtIndexPath:oldIndexPath];
	if (oldCell.accessoryType == UITableViewCellAccessoryCheckmark) {
		oldCell.accessoryType = UITableViewCellAccessoryNone;
	}
}

#pragma mark -
#pragma mark Core Data Fetched Results Controller

- (NSFetchedResultsController *)fetchedResultsController {
	
    if (_fetchedResultsController != nil) {
        return _fetchedResultsController;
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
	_fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[mCodeDescriptor release];
	[sortDescriptors release];
	
	return _fetchedResultsController;
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_managedObjectContext release];
	[_fetchedResultsController release];
	
    [super dealloc];
}

@end

