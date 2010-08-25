//
//  SymbolNewsController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 24.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "SymbolNewsController.h"

#import "SymbolNewsView_Phone.h"
#import "mTraderCommunicator.h"
#import "DataController.h"
#import "NewsArticleController_Phone.h"

#import "NewsTableViewCell_Phone.h"

#import "Feed.h"
#import "Symbol.h"
#import "NewsArticle.h"
#import "NewsFeed.h"
#import "SymbolNewsRelationship.h"

@interface SymbolNewsController ()
- (void)configureCell:(NewsTableViewCell_Phone *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (void)refresh:(id)sender;

@end

@implementation SymbolNewsController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize modal = _modal;

#pragma mark -
#pragma mark Initialization
- (id)initWithSymbol:(Symbol *)symbol {
	self = [super init];
    if (self != nil) {
		_modal = NO;
		
		_symbol = [symbol retain];
		
		_managedObjectContext = nil;
		_fetchedResultsController = nil;
		
		_headlineFont = nil;
		_bottomlineFont = nil;
		_tableView = nil;
		_newsAvailableLabel = nil;
	}
    return self;
}

- (void)loadView {
	_headlineFont = [[UIFont boldSystemFontOfSize:14.0f] retain];
	_bottomlineFont = [[UIFont systemFontOfSize:12.0f] retain];
	
	SymbolNewsView_Phone *newsView = [[SymbolNewsView_Phone alloc] initWithFrame:CGRectZero];
	newsView.modal = _modal;
	_tableView = [newsView.tableView retain];
	_tableView.delegate = self;
	_tableView.dataSource = self;
	
	_newsAvailableLabel = [newsView.newsAvailableLabel retain];
	self.view = newsView;
	[newsView release];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
	
	[self refresh:nil];
}

#pragma mark -
#pragma mark TableView datasource methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [[self.fetchedResultsController sections] count];
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:section];
	NSUInteger noOfObjects = [sectionInfo numberOfObjects];
	if (noOfObjects == 0) {
		_newsAvailableLabel.hidden = NO;
	} else {
		_newsAvailableLabel.hidden = YES;
	}
	
	return noOfObjects;
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
    
    NewsTableViewCell_Phone *cell = (NewsTableViewCell_Phone *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[NewsTableViewCell_Phone alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		cell.headlineFont = _headlineFont;
		cell.bottomlineFont = _bottomlineFont;
	}
    
	[self configureCell:cell atIndexPath:indexPath animated:NO];	
		
    return cell;
}

- (void)configureCell:(NewsTableViewCell_Phone *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	SymbolNewsRelationship *newsRelationship = (SymbolNewsRelationship *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	cell.newsArticle = newsRelationship.newsArticle;
}

#pragma mark -
#pragma mark TableView delegate methods

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NewsArticleController_Phone *newsArticleController = [[NewsArticleController_Phone alloc] init];
	
	SymbolNewsRelationship *newsRelationship = (SymbolNewsRelationship *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	newsArticleController.newsArticle = newsRelationship.newsArticle;
	
	[self.navigationController pushViewController:newsArticleController animated:YES];
	
	[newsArticleController release];
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	CGFloat windowWidth = self.view.bounds.size.width;
	CGSize constraint = CGSizeMake(windowWidth, 2000.0f);
	
	SymbolNewsRelationship *newsRelationship = (SymbolNewsRelationship *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	NSString *headline = newsRelationship.newsArticle.headline;
	NSString *feed = newsRelationship.newsArticle.newsFeed.name;
	
	CGSize headlineSize = [headline sizeWithFont:_headlineFont constrainedToSize:constraint lineBreakMode:UILineBreakModeWordWrap];
	CGSize bottomlineSize = [feed sizeWithFont:_bottomlineFont];
	
	CGFloat height = headlineSize.height + bottomlineSize.height;
	
	return height;
}

#pragma mark -
#pragma mark Managed Object Context

- (void)setManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	if (_managedObjectContext == managedObjectContext) {
		return;
	}
	[_managedObjectContext release];
	_managedObjectContext = [managedObjectContext retain];
	
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();  // Fail
#endif
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
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"SymbolNewsRelationship" inManagedObjectContext:_managedObjectContext];
	[fetchRequest setEntity:entity];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"symbol=%@", _symbol];
	[fetchRequest setPredicate:predicate];
		
	// Create the sort descriptors array.
	NSSortDescriptor *dateDescriptor = [[NSSortDescriptor alloc] initWithKey:@"newsArticle.date" ascending:NO];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:dateDescriptor, nil];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:_managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
	_fetchedResultsController = [aFetchedResultsController retain];
	_fetchedResultsController.delegate = self;
	
	// Memory management.
	[aFetchedResultsController release];
	[fetchRequest release];
	[dateDescriptor release];
	[sortDescriptors release];
	
	return _fetchedResultsController;
}

/**
 Delegate methods of NSFetchedResultsController to respond to additions, removals and so on.
 */

- (void)controllerWillChangeContent:(NSFetchedResultsController *)controller {
	// The fetch controller is about to start sending change notifications, so prepare the table view for updates.
	[_tableView beginUpdates];
}

- (void)controller:(NSFetchedResultsController *)controller didChangeObject:(id)anObject atIndexPath:(NSIndexPath *)indexPath forChangeType:(NSFetchedResultsChangeType)type newIndexPath:(NSIndexPath *)newIndexPath {
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[_tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationNone];
			break;
			
		case NSFetchedResultsChangeDelete:
			[_tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationNone];
			break;
    }
}

- (void)controller:(NSFetchedResultsController *)controller didChangeSection:(id <NSFetchedResultsSectionInfo>)sectionInfo atIndex:(NSUInteger)sectionIndex forChangeType:(NSFetchedResultsChangeType)type {
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[_tableView insertSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationNone];
			break;
			
		case NSFetchedResultsChangeDelete:
			[_tableView deleteSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationNone];
			break;
	}
}


- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
	// The fetch controller has sent all current change notifications, so tell the table view to process all updates.
	[_tableView endUpdates];
}

#pragma mark -
#pragma mark Refresh

- (void)refresh:(id)sender {
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", _symbol.feed.feedNumber, _symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] symbolNewsForFeedTicker:feedTicker];
}


#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_symbol release];
	[_managedObjectContext release];
	[_fetchedResultsController release];
	
	[_headlineFont release];
	[_bottomlineFont release];
	[_newsAvailableLabel release];
	[_tableView release];
	
    [super dealloc];
}

@end
