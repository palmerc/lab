//
//  MyListViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolAddController_Phone.h"

@class MyListTableViewController_Phone;

@interface MyListHeaderViewController_Phone : UIViewController <SymbolAddControllerDelegate> {
@private
	CGRect _frame;
	
	NSManagedObjectContext *_managedObjectContext;
	MyListTableViewController_Phone *_tableViewController;
	UINavigationController *_navigationController;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) MyListTableViewController_Phone *tableViewController;
@property (nonatomic, retain) UINavigationController *navigationController;

- (id)initWithFrame:(CGRect)frame;
- (void)changeQFieldsStreaming;
- (void)add:(id)sender;

@end
