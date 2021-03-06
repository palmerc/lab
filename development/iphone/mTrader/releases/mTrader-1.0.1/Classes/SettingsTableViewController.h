//
//  SettingsViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//
typedef enum {
	USERNAME_FIELD = 0,
	PASSWORD_FIELD
} loginEnum;

typedef enum {
	LOGINDETAILS=0,
	INFRONT
} sections;

@class mTraderCommunicator;
@class UserDefaults;

@interface SettingsTableViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate> {
	UITableView *tableView;
	UIView *aboutView;
	
	UITextField *userTextField;
	UITextField *passwordTextField;
	
	NSArray *sectionsArray;
	NSArray *infrontSectionArray;
	NSArray *loginSectionArray;
	
	UserDefaults *defaults;
	
	mTraderCommunicator *communicator;
}

@property (nonatomic, retain) UITableView *tableView;
@property (nonatomic, retain) UIView *aboutView;
@property (nonatomic, retain) UITextField *userTextField;
@property (nonatomic, retain) UITextField *passwordTextField;

@end
