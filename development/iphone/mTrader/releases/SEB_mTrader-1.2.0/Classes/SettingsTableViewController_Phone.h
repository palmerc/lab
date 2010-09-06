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
	UIView *aboutView;
	
	UITextField *userTextField;
	UITextField *passwordTextField;
	
	NSArray *_sectionsArray;
	NSArray *_infrontSectionArray;
	NSArray *_loginSectionArray;
}

@property (nonatomic, retain) UIView *aboutView;
@property (nonatomic, retain) UITextField *userTextField;
@property (nonatomic, retain) UITextField *passwordTextField;

- (void)configureCell:(UITableViewCell *)cell indexPath:(NSIndexPath *)indexPath;

@end
