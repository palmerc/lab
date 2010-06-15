//
//  SettingsViewController_Phone.h
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
@class SettingsTableViewCell;

@interface SettingsTableViewController_Phone : UITableViewController <UITextFieldDelegate> {
@private
	CGRect _frame;
	UIView *aboutView;
	
	UITextField *userTextField;
	UITextField *passwordTextField;
	
	NSArray *sectionsArray;
	NSArray *infrontSectionArray;
	NSArray *loginSectionArray;
	
	UserDefaults *defaults;
	
	mTraderCommunicator *communicator;
}

@property (nonatomic, retain) UIView *aboutView;
@property (nonatomic, retain) UITextField *userTextField;
@property (nonatomic, retain) UITextField *passwordTextField;

- (id)initWithFrame:(CGRect)frame;
- (void)configureCell:(UITableViewCell *)cell indexPath:(NSIndexPath *)indexPath;

@end
