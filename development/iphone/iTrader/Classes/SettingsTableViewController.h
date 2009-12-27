//
//  SettingsViewController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
	USERNAME_FIELD = 0,
	PASSWORD_FIELD = 1
} loginEnum;

typedef enum {
	LOGIN=0,
	CURRENCY=1,
	INFRONT=2
} sections;

@interface SettingsTableViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate> {
	UITableView *tableView;
	UIView *aboutView;
	
	NSUserDefaults *defaults;
	
	NSArray *sectionsArray;
	NSArray *infrontSectionArray;
	NSArray *loginSectionArray;
}

@property (nonatomic, retain) UITableView *tableView;
@property (nonatomic, retain) UIView *aboutView;

@end
