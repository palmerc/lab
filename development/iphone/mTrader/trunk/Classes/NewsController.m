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
#import "Feed.h"
#import "Symbol.h"
#import "NewsCell.h"
#import "NewsItemController.h"

@implementation NewsController
@synthesize communicator, managedObjectContext, fetchedResultsController;
@synthesize mCode = _mCode;

#pragma mark -
#pragma mark Initialization

- (id)init {
	self = [super init];
    if (self != nil) {
		self.title = NSLocalizedString(@"NewsTab", "News tab label");
		UIImage* anImage = [UIImage imageNamed:@"newsTabButton.png"];	
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"NewsTab", "News tab label")  image:anImage tag:NEWS];
		self.tabBarItem = theItem;
		[theItem release];
		
		newsArray = nil;
		
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

	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
	
	communicator = [mTraderCommunicator sharedManager];
	[communicator stopStreamingData];
	communicator.symbolsDelegate = self;
	[communicator newsListFeed:self.mCode];
}

- (void)viewDidLoad {	
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
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [newsArray count];
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
	NSInteger row = indexPath.row;
	NSArray *newsItem = [newsArray objectAtIndex:row];
	cell.newsItem = newsItem;
}

#pragma mark -
#pragma mark TableView delegate methods

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NewsItemController *newsItemController = [[NewsItemController alloc] init];
	
	NSArray *articleArray = [newsArray objectAtIndex:indexPath.row];
	NSString *feedArticle = [articleArray objectAtIndex:0];
	newsItemController.feedArticle = feedArticle;
	
	[self.navigationController pushViewController:newsItemController animated:YES];
	
	[newsItemController release];
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

#pragma mark -
#pragma mark UIPickerViewDelegate Methods
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	NewsFeed *feed = (NewsFeed *)[fetchedResultsController objectAtIndexPath:indexPath];
	self.mCode = feed.mCode;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	NewsFeed *feed = (NewsFeed *)[fetchedResultsController objectAtIndexPath:indexPath];
	NSString *feedName = feed.name;
	return feedName;
}

#pragma mark -
#pragma mark mTraderCommunicatorDelegate methods

-(void) newsListFeedsUpdates:(NSArray *)newsList {
	if (newsArray != nil) {
		[newsArray release];
		newsArray = nil;
	}
	
	NSMutableArray *tempStorage = [[[NSMutableArray alloc] init] autorelease];
	for (NSString *news in newsList) {
		NSArray *components = [news componentsSeparatedByString:@";"];
		if ([components count] >= 4) {
			NSString *feedArticle = [components objectAtIndex:0];
			NSString *flag = [components objectAtIndex:1];
			NSString *date = [components objectAtIndex:2];
			NSString *time = [components objectAtIndex:3];
			NSString *headline = [components objectAtIndex:4];
			NSArray *tuple = [NSArray arrayWithObjects:feedArticle, flag, date, time, headline, nil];
			
			[tempStorage addObject:tuple];
		}
	}
	
	newsArray = (NSArray *)tempStorage;
	[newsArray retain];
	
	[table reloadData];
}

#pragma mark -
#pragma mark Actions

- (void)refresh:(id)sender {
	[newsArray release];
	newsArray = nil;
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
		[newsArray release];
		newsArray = nil;
		[communicator newsListFeed:self.mCode];
	} else if (buttonIndex == 2) {
		[newsArray release];
		newsArray = nil;
		[communicator newsListFeed:self.mCode];
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
	[_mCode release];
	[table release];
	[newsArray release];
    [super dealloc];
}


@end
