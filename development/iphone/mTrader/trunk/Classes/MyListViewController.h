//
//  MyListViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 21.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "SymbolAddController.h"

@class ChainsTableViewController;

@interface MyListViewController : UIViewController <SymbolAddControllerDelegate> {
@private
	CGRect _frame;
	
	NSManagedObjectContext *_managedObjectContext;

	ChainsTableViewController *_tableViewController;
	UINavigationController *_navigationController;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) ChainsTableViewController *tableViewController;
@property (nonatomic, retain) UINavigationController *navigationController;

- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)changeQFieldsStreaming;
- (void)add:(id)sender;

@end
