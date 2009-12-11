//
//  PersonListViewController.m
//  Presence
//
//  Created by Cameron Lowell Palmer on 09.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "PersonListViewController.h"
#import "PersonDetailViewController.h"
#import "Person.h"


@implementation PersonListViewController

- (id)initWithStyle:(UITableViewStyle)style
{
	self = [super initWithStyle:style];
	if (self != nil) {
		NSArray *persons = [Person initWithNames:@"Cameron", @"Mari", @"Jessica", @"Ryan", @"Atticus"]
		
		people = [[NSMutableArray alloc] initWithArray:persons];
		
	}
	return self;
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	self.title = @"People";
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


- (void)dealloc {
	[people release];
    [super dealloc];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [people count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"PersonListCell"];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"PersonListCell"] autorelease];
	}
	
	NSString *person = [people objectAtIndex:indexPath.row];
	NSString *filename = [[NSString alloc] initWithFormat:@"%@.jpg", person.lowercaseString];
	[cell.imageView setImage:[UIImage imageNamed:filename]];
	[cell.textLabel setText:person];
	
	return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	PersonDetailViewController *personDetailVC = [[PersonDetailViewController alloc] initWithNibName:@"PersonDetailView" bundle:nil];
	[self.navigationController pushViewController:personDetailVC animated:YES];
	
	[personDetailVC release];
}

@end
