//
//  NewsViewController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "NewsViewController.h"
#import "NewsItemViewController.h"
#import "iTraderAppDelegate.h"
#import "iTraderCommunicator.h"

@implementation NewsViewController
@synthesize communicator;
@synthesize previousmTraderServerDataDelegate;
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
		
		self.newsArray = [[NSMutableArray alloc] init];
		communicator = [iTraderCommunicator sharedManager];
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

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	UIBarButtonItem *refreshNews = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(refreshNews)];
	self.navigationItem.rightBarButtonItem = refreshNews;
	[refreshNews release];
	
	self.previousmTraderServerDataDelegate = self.communicator.mTraderServerDataDelegate;
	
	self.communicator.mTraderServerDataDelegate = self;
	[self.communicator newsListFeeds];
}



// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait);
	return YES;
}


- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

-(void) viewWillDisappear:(BOOL)animated {
	self.communicator.mTraderServerDataDelegate = self.previousmTraderServerDataDelegate;
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
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


#pragma mark TableViewDelegate Methods
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NSUInteger row = indexPath.row;
	
	NSArray *tuple = [self.newsArray objectAtIndex:row];
	NSString *newsID = [tuple objectAtIndex:0];
	NewsItemViewController *newsItemViewController = [[NewsItemViewController alloc] initWithNewsItem:newsID];
	[self.navigationController pushViewController:newsItemViewController animated:YES];
	
}

#pragma mark Delegation

-(void) newsListFeedsUpdates:(NSArray *)newsList {
	for (NSString *news in newsList) {
		NSArray *components = [news componentsSeparatedByString:@";"];
		NSString *key = [components objectAtIndex:0];
		NSString *value = [components objectAtIndex:4];
		NSArray *tuple = [NSArray arrayWithObjects:key, value, nil];
		
		[self.newsArray addObject:tuple];
	}
	
	[self.tableView reloadData];
}

#pragma mark News Refresh

-(void) refreshNews {
	[self.newsArray removeAllObjects];
	[self.communicator newsListFeeds];
	[self.tableView reloadData];
}

@end
