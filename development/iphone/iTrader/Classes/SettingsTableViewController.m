//
//  SettingsViewController.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "SettingsTableViewController.h"
#import "AboutViewController.h"
#import "iTraderAppDelegate.h"
#import "iTraderCommunicator.h"

@implementation SettingsTableViewController
@synthesize tableView;
@synthesize aboutView;
@synthesize userTextField;
@synthesize passwordTextField;

- (id)init {
	self = [super init];
	if (self != nil) {
		defaults = [[NSUserDefaults alloc] init];
		communicator = [iTraderCommunicator sharedManager];
		
		UIImage* anImage = [UIImage imageNamed:@"infront.png"];
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"SettingsTab", @"The settings tab label") image:anImage tag:SETTINGS];
		self.tabBarItem = theItem;
		[theItem release];
		
		sectionsArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"LoginSettings", @"Login settings for infront account"), NSLocalizedString(@"Currency", "@Currency preference"), NSLocalizedString(@"Company", @"Company name"), nil];
		loginSectionArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"Username", @"Username string"), NSLocalizedString(@"Password", @"Password string"), nil];
		infrontSectionArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"About", @"About string"), nil];
		self.title = NSLocalizedString(@"SettingsTab", @"The settings tab label");
		
		tableView = [[UITableView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame] style:UITableViewStyleGrouped];
		tableView.delegate = self;
		tableView.dataSource = self;
		self.view = tableView;
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

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];


}
*/

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
	[defaults release];
    [super dealloc];
}

// Table View Data Source Methods
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [sectionsArray count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	if (section == LOGIN) {
		return [loginSectionArray count];
	} else if (section == INFRONT) {
		return [infrontSectionArray count];
	} else {
		return 0;
	}
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	return [sectionsArray objectAtIndex:section];
}

- (UITableViewCell *)tableView:(UITableView *)_tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *CellIdentifier = @"SettingsCell";
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
	}
	if (indexPath.section == LOGIN) {
		[cell.textLabel setText:[loginSectionArray objectAtIndex:indexPath.row]];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		
		UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(110, 5, 190, 35)];
		textField.delegate = self;
		textField.autocorrectionType = UITextAutocorrectionTypeNo;
		textField.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
		textField.textColor = [[UIColor alloc] initWithRed:.25 green:.35 blue:.55 alpha:1];
		textField.autocapitalizationType = UITextAutocapitalizationTypeNone;
		textField.borderStyle = UITextBorderStyleNone;
		textField.returnKeyType = UIReturnKeyDone;
		if (indexPath.row == USERNAME_FIELD) {
			textField.tag = USERNAME_FIELD;
			textField.text = [defaults stringForKey:@"username"];
			self.userTextField = textField;
		} else if (indexPath.row == PASSWORD_FIELD) {
			textField.secureTextEntry = YES;
			textField.clearButtonMode = UITextFieldViewModeWhileEditing;
			textField.tag = PASSWORD_FIELD;
			textField.text = [defaults stringForKey:@"password"];
			self.passwordTextField = textField;
		}
		
		[cell addSubview:textField];
		
	} else if (indexPath.section == INFRONT) {
		[cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
		[cell.textLabel setText:[infrontSectionArray objectAtIndex:indexPath.row]];
	} else {
		[cell.textLabel setText:@"Hello, Dolly!"];
	}
	return cell;
}

// Table View Delegate Method
- (void)tableView:(UITableView *)_tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	if (indexPath.section == LOGIN) {
		if (indexPath.row == 0) {
		} else if (indexPath.row == 1) {
		}
	} else if (indexPath.section == INFRONT) {
		[_tableView deselectRowAtIndexPath:indexPath animated:NO];
		AboutViewController *aboutViewController = [[AboutViewController alloc] initWithNibName:@"AboutView" bundle:nil];
		[self.navigationController pushViewController:aboutViewController animated:YES];
		
		[aboutViewController release];
	}	
}

// Text Field Delegate Methods
- (void)textFieldDidEndEditing:(UITextField *)textField {
	if (textField.tag == USERNAME_FIELD) {
		[defaults setObject:textField.text forKey:@"username"];
	} else if (textField.tag == PASSWORD_FIELD) {
		[defaults setObject:textField.text forKey:@"password"];
	}
	
	NSString *username = self.userTextField.text;
	NSString *password = self.passwordTextField.text;
	// As long as username and password are not empty or nil attempt to connect
	if (username != nil && password != nil) {
		[communicator login:username password:password];
	}
}

-(BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
	return YES;
}

@end
