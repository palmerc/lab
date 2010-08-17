//
//  PastTradesController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "PastTradesController.h"

#import "PastTradesView.h"
#import <QuartzCore/QuartzCore.h>
#import "mTraderCommunicator.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Feed.h"
#import "Trade.h"

#import "TradesCell.h"

@interface PastTradesController ()
- (void)configureCell:(TradesCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (void)refresh:(id)sender;
@end

@implementation PastTradesController
@synthesize managedObjectContext = _managedObjectContext;
@synthesize fetchedResultsController = _fetchedResultsController;

- (id)initWithSymbol:(Symbol *)symbol {
	self = [super init];
	if (self != nil) {
		_symbol = [symbol retain];
	
		_tableView = nil;
		
		_managedObjectContext = nil;
		_fetchedResultsController = nil;
	}
	return self;
}

- (void)loadView {
	PastTradesView *pastTradesView = [[PastTradesView alloc] initWithFrame:CGRectZero];
	self.view = pastTradesView;
	_tableView = [pastTradesView.tableView retain];
	_tableView.delegate = self;
	_tableView.dataSource = self;
	[pastTradesView release];	
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	[self refresh:nil];
}

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
#pragma mark TableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [[_fetchedResultsController sections] count];
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	id <NSFetchedResultsSectionInfo> sectionInfo = [[_fetchedResultsController sections] objectAtIndex:section];
    NSUInteger rowCount = [sectionInfo numberOfObjects];
	if (rowCount == 0) {
		_tradesAvailableLabel.hidden = NO;
	} else {
		_tradesAvailableLabel.hidden = YES;
	}
	return rowCount;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	NSInteger count = [[_fetchedResultsController sections] count];
	if (count > 0) {
		id <NSFetchedResultsSectionInfo> sectionInfo = [[_fetchedResultsController sections] objectAtIndex:section];
		return [sectionInfo name];
	}
	return nil;	
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"TradesCell";
    
    TradesCell *cell = (TradesCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[TradesCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;

		UIFont *mainFont = [UIFont systemFontOfSize:12.0f];
		CGSize mainFontSize = [@"X" sizeWithFont:mainFont];
		
		cell.mainFont = mainFont;
		
		CGFloat fifthWidth = floorf(self.view.bounds.size.width / 5.0f);
		CGRect timeLabelFrame = CGRectMake(0.0f, 0.0f, fifthWidth, mainFontSize.height);
		CGRect priceLabelFrame = CGRectMake(fifthWidth, 0.0f, fifthWidth, mainFontSize.height);
		CGRect volumeLabelFrame = CGRectMake(fifthWidth * 2.0f, 0.0f, fifthWidth, mainFontSize.height);
		CGRect buyerLabelFrame = CGRectMake(fifthWidth * 3.0f, 0.0f, fifthWidth, mainFontSize.height);
		CGRect sellerLabelFrame = CGRectMake(fifthWidth * 4.0f, 0.0f, fifthWidth, mainFontSize.height);
								
		cell.timeLabel.frame = timeLabelFrame;
		cell.priceLabel.frame = priceLabelFrame;
		cell.volumeLabel.frame = volumeLabelFrame;
		cell.buyerLabel.frame = buyerLabelFrame;
		cell.sellerLabel.frame = sellerLabelFrame;
	}
    
    // Configure the cell.
	[self configureCell:cell atIndexPath:indexPath animated:NO];
    return cell;
}

- (void)configureCell:(TradesCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
	Trade *trade = (Trade *)[_fetchedResultsController objectAtIndexPath:indexPath];
	
	cell.trade = trade;
}

#pragma mark -
#pragma mark UITableViewDelegate methods
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	UIFont *mainFont = [UIFont systemFontOfSize:14.0f];
	CGSize mainSize = [@"X" sizeWithFont:mainFont];
	
	return mainSize.height;
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
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"Trade" inManagedObjectContext:self.managedObjectContext];
	[fetchRequest setEntity:entity];
	
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"symbol=%@", _symbol];
	[fetchRequest setPredicate:predicate];
	
	// Create the sort descriptors array.
	NSSortDescriptor *dateDescriptor = [[NSSortDescriptor alloc] initWithKey:@"time" ascending:NO];
	NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:dateDescriptor, nil];
	[fetchRequest setSortDescriptors:sortDescriptors];
	
	// Create and initialize the fetch results controller.
	NSFetchedResultsController *aFetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:self.managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
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
	UITableView *tableView = _tableView;
	
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
	[[mTraderCommunicator sharedManager] tradesRequest:feedTicker];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[_managedObjectContext release];
	[_fetchedResultsController release];
	[_symbol release];
	[_tradesAvailableLabel release];
	[_tableView release];

    [super dealloc];
}


@end

