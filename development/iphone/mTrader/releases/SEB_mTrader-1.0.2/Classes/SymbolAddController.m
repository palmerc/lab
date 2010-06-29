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
@synthesize fetchedResultsController = _fetchedResultsController;
@synthesize managedObjectContext = _managedObjectContext;
@synthesize tickerField = _tickerField;
@synthesize submitButton = _submitButton;
@synthesize exchangePicker = _exchangePicker;
@synthesize mCode = _mCode;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
	if (self != nil) {
		_managedObjectContext = nil;
	}
	return self;
}
 
#pragma mark -
#pragma mark Application Lifecycle

- (void)viewDidLoad {
	[super viewDidLoad];
	
	self.title = @"Add a Symbol";
	
	//self.searchBar.delegate = self;
	//self.searchBar.placeholder = @"Stock Ticker Symbol";
	UIBarButtonItem *cancelItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancel:)]; 
	self.navigationItem.leftBarButtonItem = cancelItem;
	
	_communicator = [mTraderCommunicator sharedManager];
	
	[self.submitButton setTitle:@"Add" forState:UIControlStateNormal];
	
	self.tickerField.delegate = self;
	self.tickerField.clearButtonMode = UITextFieldViewModeWhileEditing;
	self.tickerField.autocorrectionType = UITextAutocorrectionTypeNo;
	self.tickerField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
	
	// Core Data Setup - This not only grabs the existing results but also setups up the FetchController
	NSError *error;
	if (![self.fetchedResultsController performFetch:&error]) {
		// Update to handle the error appropriately.
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
#if DEBUG
		abort();  // Fail
#endif
	}

	id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:0];
	NSInteger count = [sectionInfo numberOfObjects];
	NSInteger ossIndex = 0;
	for (int i = 0; i < count; i++) {
		NSIndexPath *indexPath = [NSIndexPath indexPathForRow:i inSection:0];
		Feed *feed = (Feed *)[self.fetchedResultsController objectAtIndexPath:indexPath];
		if ([feed.mCode isEqualToString:@"OSS"]) {
			self.mCode = @"OSS";
			ossIndex = i;
			break;
		}
	}
	
	[self.exchangePicker selectRow:ossIndex inComponent:0 animated:NO];	
}

#pragma mark -
#pragma mark UIPickerViewDataSource Required Methods
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
	return [[self.fetchedResultsController sections] count];
}


- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
	id <NSFetchedResultsSectionInfo> sectionInfo = [[self.fetchedResultsController sections] objectAtIndex:component];
    return [sectionInfo numberOfObjects];
}

#pragma mark -
#pragma mark UIPickerViewDelegate Methods
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	Feed *feed = (Feed *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	self.mCode = feed.mCode;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
	NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:component];
	Feed *feed = (Feed *)[self.fetchedResultsController objectAtIndexPath:indexPath];
	NSString *feedName = feed.feedName;
	return feedName;
}

#pragma mark -
#pragma mark IBAction Section
-(IBAction) submit:(id)sender {
	[self.tickerField resignFirstResponder];
	
	NSString *tickerSymbol = self.tickerField.text;
	if ([tickerSymbol isEqualToString:@""]) {
		[self.delegate symbolAddControllerDidFinish:self didAddSymbol:nil];
	} else {
		[_communicator addSecurity:tickerSymbol withMCode:self.mCode];
		[self.delegate symbolAddControllerDidFinish:self didAddSymbol:tickerSymbol];
	}
}

- (void)cancel:(id)sender {
	[self.tickerField resignFirstResponder];	
	[self.delegate symbolAddControllerDidFinish:self didAddSymbol:nil];
}


#pragma mark -
#pragma mark UITextFieldDelegate methods
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
    if (_fetchedResultsController != nil) {
        return _fetchedResultsController;
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
	_fetchedResultsController = aFetchedResultsController;
	_fetchedResultsController.delegate = self;
	
	// Memory management.
	[fetchRequest release];
	[mCodeDescriptor release];
	[sortDescriptors release];
	
	return _fetchedResultsController;
}

#pragma mark -
#pragma mark Debugging methods
/*
 // Very helpful debug when things seem not to be working.
 - (BOOL)respondsToSelector:(SEL)sel {
 NSLog(@"Queried about %@", NSStringFromSelector(sel));
 return [super respondsToSelector:sel];
 }
*/

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[self.fetchedResultsController release];
	[_managedObjectContext release];
	[self.submitButton release];
	[self.tickerField release];
	[self.exchangePicker release];
	[self.mCode release];
    [super dealloc];
}


@end