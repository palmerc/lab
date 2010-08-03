//
//  SymbolNewsModalController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 24.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "SymbolNewsController_Phone.h"

#import "mTraderCommunicator.h"
#import "DataController.h"
#import "NewsArticleController_Phone.h"

#import "NewsTableViewCell_Phone.h"

#import "Feed.h"
#import "Symbol.h"
#import "NewsArticle.h"
#import "SymbolNewsRelationship.h"

@implementation SymbolNewsController_Phone
@synthesize managedObjectContext = _managedObjectContext;
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize symbol = _symbol;
@synthesize newsAvailableLabel = _newsAvailableLabel;

#pragma mark -
#pragma mark Initialization
- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	self = [super init];
    if (self != nil) {
		_managedObjectContext = [managedObjectContext retain];
		_fetchedResultsController = nil;
		_newsAvailable = NO;
		_symbol = nil;
		
		_newsAvailableLabel = nil;
	}
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	
	UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
	self.navigationItem.leftBarButtonItem = doneButton;
	[doneButton release];
	
	UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refresh:)];
	self.navigationItem.rightBarButtonItem = refreshButton;
	[refreshButton release];
	
	NSString *labelString = NSLocalizedString(@"noNewsAvailable", @"No News Available");
	UIFont *labelFont = [UIFont boldSystemFontOfSize:24.0f];
	CGRect frame = self.view.bounds;
	frame.size.height = [labelString sizeWithFont:labelFont].height;
	
	_newsAvailableLabel = [[UILabel alloc] initWithFrame:frame];
	_newsAvailableLabel.textAlignment = UITextAlignmentCenter;
	_newsAvailableLabel.font = labelFont;
	_newsAvailableLabel.textColor = [UIColor blackColor];
	_newsAvailableLabel.backgroundColor = [UIColor clearColor];
	_newsAvailableLabel.text = labelString;
	_newsAvailableLabel.hidden = YES;
	[self.tableView addSubview:self.newsAvailableLabel];
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
		self.newsAvailableLabel.hidden = NO;
	} else {
		self.newsAvailableLabel.hidden = YES;
	}
	
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
    
    NewsTableViewCell_Phone *cell = (NewsTableViewCell_Phone *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[NewsTableViewCell_Phone alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
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

- (void)setSymbol:(Symbol *)symbol {
	if (symbol == _symbol) {
		return;
	}
	
	[_symbol release];
	_symbol = [symbol retain];
	
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
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"SymbolNewsRelationship" inManagedObjectContext:self.managedObjectContext];
	[fetchRequest setEntity:entity];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"symbol=%@", _symbol];
	[fetchRequest setPredicate:predicate];
		
	// Create the sort descriptors array.
	NSSortDescriptor *dateDescriptor = [[NSSortDescriptor alloc] initWithKey:@"newsArticle.date" ascending:NO];
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
			[tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationNone];
			break;
			
		case NSFetchedResultsChangeDelete:
			[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationNone];
			break;
    }
}


- (void)controller:(NSFetchedResultsController *)controller didChangeSection:(id <NSFetchedResultsSectionInfo>)sectionInfo atIndex:(NSUInteger)sectionIndex forChangeType:(NSFetchedResultsChangeType)type {
	
	switch(type) {
			
		case NSFetchedResultsChangeInsert:
			[self.tableView insertSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationNone];
			break;
			
		case NSFetchedResultsChangeDelete:
			[self.tableView deleteSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationNone];
			break;
	}
}


- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
	// The fetch controller has sent all current change notifications, so tell the table view to process all updates.
	[self.tableView endUpdates];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_symbol release];
	[_managedObjectContext release];
	[_fetchedResultsController release];
	
    [super dealloc];
}

@end
