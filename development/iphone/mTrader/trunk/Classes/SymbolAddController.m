//
//  SymbolAddController.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolAddController.h"

#import <UIKit/UITableView.h>

#import "Feed.h"
#import "mTraderCommunicator.h"



@implementation SymbolAddController
@synthesize delegate;
@synthesize fetchedResultsController, managedObjectContext;
@synthesize tickerField = _tickerField;
@synthesize submitButton = _submitButton;
@synthesize exchangePicker = _exchangePicker;
@synthesize tickerSymbol;
@synthesize mCode;
 
#pragma mark -
#pragma mark Application Lifecycle

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	// Core Data Setup - This not only grabs the existing results but also setups up the FetchController
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		abort();  // Fail
	}
	
	//self.searchBar.delegate = self;
	//self.searchBar.placeholder = @"Stock Ticker Symbol";
	//self.searchBar.showsCancelButton = YES;
	
	communicator = [mTraderCommunicator sharedManager];
	
	[self.submitButton setTitle:@"Cancel" forState:UIControlStateNormal];
	
	[self.tickerField addTarget:self action:@selector(editing:) forControlEvents:UIControlEventEditingChanged];
	self.tickerField.delegate = self;
	self.tickerField.clearButtonMode = UITextFieldViewModeWhileEditing;
	self.tickerField.autocorrectionType = UITextAutocorrectionTypeNo;
	self.tickerField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
	
	id <NSFetchedResultsSectionInfo> sectionInfo = [[fetchedResultsController sections] objectAtIndex:0];
    NSInteger count = [sectionInfo numberOfObjects];
	NSInteger ossIndex = 0;
	for (int i = 0; i < count; i++) {
		NSIndexPath *indexPath = [NSIndexPath indexPathForRow:i inSection:0];
		Feed *feed = (Feed *)[fetchedResultsController objectAtIndexPath:indexPath];
		if ([feed.mCode isEqualToString:@"OSS"]) {
			self.mCode = @"OSS";
			ossIndex = i;
			break;
		}
	}
	[self.exchangePicker selectRow:ossIndex inComponent:0 animated:NO];
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	fetchedResultsController = nil;
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
	NSLog(@"You selected row %d in component %d.", row, component);
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	Feed *feed = (Feed *)[fetchedResultsController objectAtIndexPath:indexPath];
	self.mCode = feed.mCode;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	Feed *feed = (Feed *)[fetchedResultsController objectAtIndexPath:indexPath];
	NSString *feedName = feed.feedName;
	return feedName;
}

#pragma mark -
#pragma mark IBAction Section
-(IBAction) submit:(id)sender {
	[self.tickerField resignFirstResponder];
	self.tickerSymbol = self.tickerField.text;
	if ([self.tickerSymbol isEqualToString:@""]) {
		[self.delegate symbolAddControllerDidFinish:self didAddSymbol:nil];
	} else {
		[communicator addSecurity:self.tickerSymbol withMCode:self.mCode];
		[self.delegate symbolAddControllerDidFinish:self didAddSymbol:self.tickerSymbol];
	}
}

#pragma mark -
#pragma mark UITextFieldDelegate methods

- (void)editing:(id)sender {
	NSLog(@"HERE %@", sender);
	UITextField *textField = sender;
	if (![textField.text isEqualToString:@""]) {
		[self.submitButton setTitle:@"Submit" forState:UIControlStateNormal];
	} else {
		[self.submitButton setTitle:@"Cancel" forState:UIControlStateNormal];
	}
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return YES;
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
	NSEntityDescription *entity = [NSEntityDescription entityForName:@"Feed" inManagedObjectContext:self.managedObjectContext];
	[fetchRequest setEntity:entity];
	
	// Create the sort descriptors array.
	NSSortDescriptor *mCodeDescriptor = [[NSSortDescriptor alloc] initWithKey:@"feedName" ascending:YES];
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


#pragma mark -
#pragma mark Debugging methods

 // Very helpful debug when things seem not to be working.
 - (BOOL)respondsToSelector:(SEL)sel {
 NSLog(@"Queried about %@", NSStringFromSelector(sel));
 return [super respondsToSelector:sel];
 }


#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [super dealloc];
}


@end
