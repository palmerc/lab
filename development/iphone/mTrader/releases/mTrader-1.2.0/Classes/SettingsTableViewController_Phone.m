//
//  SettingsTableViewController_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import "SettingsTableViewController_Phone.h"

#import "mTraderAppDelegate_Phone.h"
#import "AboutViewController_Phone.h"

#import "mTraderCommunicator.h"
#import "Monitor.h"
#import "UserDefaults.h"
#import "QFields.h"

@implementation SettingsTableViewController_Phone
@synthesize aboutView;
@synthesize userTextField;
@synthesize passwordTextField;

#pragma mark -
#pragma mark Application Initialization

- (id)init {
	self = [super initWithStyle:UITableViewStyleGrouped];
	if (self != nil) {		
		UIImage* anImage = [UIImage imageNamed:@"SettingsTab.png"];
		UITabBarItem* theItem = [[UITabBarItem alloc] initWithTitle:NSLocalizedString(@"SettingsTab", @"The settings tab label") image:anImage tag:SETTINGS];
		self.tabBarItem = theItem;
		[theItem release];
	}
	return self;
}

#pragma mark -
#pragma mark UIViewController Methods

- (void)viewDidLoad {
	[super viewDidLoad];
	self.title = NSLocalizedString(@"SettingsTab", @"The settings tab label");

	_sectionsArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"LoginSettings", @"Login settings for infront account"), NSLocalizedString(@"Company", @"Company name"), nil];
	_loginSectionArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"Username", @"Username string"), NSLocalizedString(@"Password", @"Password string"), nil];
	_infrontSectionArray = [[NSArray alloc] initWithObjects:NSLocalizedString(@"About", @"About string"), nil];

	mTraderCommunicator *communicator = [mTraderCommunicator sharedManager];
	QFields *qFields = [[QFields alloc] init];
	communicator.qFields = qFields;
	[qFields release];
	
	[communicator setStreamingForFeedTicker:nil];
}

#pragma mark -
#pragma mark UITableView DataSource Methods

// Table View Data Source Methods
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [_sectionsArray count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	if (section == LOGINDETAILS) {
		return [_loginSectionArray count];
	} else if (section == INFRONT) {
		return [_infrontSectionArray count];
	} else {
		return 0;
	}
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	return [_sectionsArray objectAtIndex:section];
}

- (UITableViewCell *)tableView:(UITableView *)_tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *CellIdentifier = @"SettingsCell_Phone";
	UITableViewCell *cell = [self.tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
	}
	
	[self configureCell:cell indexPath:indexPath];
	
	return cell;
}

- (void)configureCell:(UITableViewCell *)cell indexPath:(NSIndexPath *)indexPath{
	if (indexPath.section == LOGINDETAILS) {
		UserDefaults *defaults = [UserDefaults sharedManager];

		NSString *aText = [_loginSectionArray objectAtIndex:indexPath.row];
		[cell.textLabel setText:aText];
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		
		// TODO - This should be fixed to be dynamic
		CGRect frame = CGRectMake(110.0f, 0.0f, cell.frame.size.width - 130.0f, 44.0f);
		UITextField *textField = [[UITextField alloc] initWithFrame:frame];
		textField.delegate = self;
		textField.autocorrectionType = UITextAutocorrectionTypeNo;
		textField.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
		
		UIColor *textColor = [[UIColor alloc] initWithRed:.25 green:.35 blue:.55 alpha:1];
		textField.textColor = textColor;
		[textColor release];
		
		textField.autocapitalizationType = UITextAutocapitalizationTypeNone;
		textField.borderStyle = UITextBorderStyleNone;
		textField.returnKeyType = UIReturnKeyDone;
		if (indexPath.row == USERNAME_FIELD) {
			textField.tag = USERNAME_FIELD;
			textField.text = defaults.username;
			self.userTextField = textField;
		} else if (indexPath.row == PASSWORD_FIELD) {
			textField.secureTextEntry = YES;
			textField.clearButtonMode = UITextFieldViewModeWhileEditing;
			textField.tag = PASSWORD_FIELD;
			textField.text = defaults.password;
			self.passwordTextField = textField;
		}
		
		[cell.contentView addSubview:textField];
		[textField release];
		
	} else if (indexPath.section == INFRONT) {
		[cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
		[cell.textLabel setText:[_infrontSectionArray objectAtIndex:indexPath.row]];
	} else {
		[cell.textLabel setText:@"Hello, Dolly!"];
	}
}

#pragma mark -
#pragma mark UITableView Delegate Methods

// Table View Delegate Method
- (void)tableView:(UITableView *)_tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	if (indexPath.section == LOGINDETAILS) {
		if (indexPath.row == 0) {
		} else if (indexPath.row == 1) {
		}
	} else if (indexPath.section == INFRONT) {
		[_tableView deselectRowAtIndexPath:indexPath animated:NO];
		AboutViewController_Phone *aboutViewController = [[AboutViewController_Phone alloc] initWithNibName:@"AboutView_Phone" bundle:nil];
		[self.navigationController pushViewController:aboutViewController animated:YES];
		
		[aboutViewController release];
	}	
}

#pragma mark -
#pragma mark UITextField Delegate Methods

// Text Field Delegate Methods
- (void)textFieldDidEndEditing:(UITextField *)textField {
	UserDefaults *defaults = [UserDefaults sharedManager];

	NSString *username = defaults.username;
	NSString *password = defaults.password;
	
	BOOL update = NO;
	if (textField.tag == USERNAME_FIELD) {
		if (![username isEqualToString:textField.text]) {
			username = textField.text;
			update = YES;
		}
	} else if (textField.tag == PASSWORD_FIELD) {
		if (![password isEqualToString:textField.text]) {
			password = textField.text;
			update = YES;
		}
	}
	
	if (update == YES) {
		defaults.username = username;
		defaults.password = password;
		[defaults saveSettings];
	}
	
	// As long as username and password are not empty or nil attempt to connect
	[[Monitor sharedManager] usernameAndPasswordChanged];
}

-(BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
	return YES;
}

#pragma mark -
#pragma mark Memory Management

- (void)dealloc {
	[_sectionsArray release];
	[_loginSectionArray release];
	[_infrontSectionArray release];
	
    [super dealloc];
}

@end
