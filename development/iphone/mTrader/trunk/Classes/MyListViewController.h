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
	NSManagedObjectContext *_managedObjectContext;

	ChainsTableViewController *_tableViewController;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) ChainsTableViewController *tableViewController;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)add:(id)sender;

@end
