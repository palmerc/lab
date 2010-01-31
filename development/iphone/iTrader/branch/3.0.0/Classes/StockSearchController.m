//
//  StockSearchController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "StockSearchController.h"
#import "iTraderCommunicator.h"
#import "SymbolsController.h"

@implementation StockSearchController
@synthesize delegate;
@synthesize tickerField = _tickerField;
@synthesize submitButton = _submitButton;
@synthesize exchangePicker = _exchangePicker;
@synthesize tickerSymbol;
@synthesize mCode;

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
	
	
	
	//self.searchBar.delegate = self;
	//self.searchBar.placeholder = @"Stock Ticker Symbol";
	//self.searchBar.showsCancelButton = YES;
	
	controller = [SymbolsController sharedManager];
	communicator = [iTraderCommunicator sharedManager];
	
	self.tickerField.delegate = self;
	self.tickerField.clearButtonMode = UITextFieldViewModeWhileEditing;
	self.tickerField.autocorrectionType = UITextAutocorrectionTypeNo;
	self.tickerField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
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
    [super dealloc];
}

/*
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
    NSLog(@"Queried about %@", NSStringFromSelector(sel));
    return [super respondsToSelector:sel];
}
*/

#pragma mark -
#pragma mark UIPickerViewDataSource Required Methods
-(NSInteger) numberOfComponentsInPickerView:(UIPickerView *)pickerView {
	return 1;
}
-(NSInteger) pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
	return [controller.exchanges count];
}

#pragma mark -
#pragma mark UIPickerViewDelegate Methods
-(void) pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
	//NSLog(@"Picked %@", [controller.exchanges objectAtIndex:row]);
	self.mCode = [controller.exchanges objectAtIndex:row];
}

-(NSString *) pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
	return [controller.exchanges objectAtIndex:row];
}

-(BOOL) textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return YES;
}

#pragma mark -
#pragma mark IBAction Section
-(IBAction) submit:(id)sender {
	[self.tickerField resignFirstResponder];
	self.tickerSymbol = self.tickerField.text;
	if ([self.tickerSymbol isEqualToString:@""]) {
		[self.delegate stockSearchControllerDidFinish:self didAddSymbol:nil];
	} else {
		[communicator addSecurity:self.tickerSymbol withMCode:self.mCode];
		[self.delegate stockSearchControllerDidFinish:self didAddSymbol:tickerSymbol];
	}
}

- (void)addFailedAlreadyExists {
	NSString *alertTitle = @"Add Security Failed";
	NSString *alertMessage = @"The ticker symbol you requested is already in your list.";
	NSString *alertCancel = @"Dismiss";
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertTitle message:alertMessage delegate:nil cancelButtonTitle:alertCancel otherButtonTitles:nil];
	[alertView show];	
}

- (void)addFailedNotFound {
	NSString *alertTitle = @"Add Security Failed";
	NSString *alertMessage = @"The ticker symbol you requested was not found.";
	NSString *alertCancel = @"Dismiss";
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertTitle message:alertMessage delegate:nil cancelButtonTitle:alertCancel otherButtonTitles:nil];
	[alertView show];
}

@end
