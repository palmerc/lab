//
//  ChainsNavigationViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class ChainsTableViewController;

@interface ChainsNavigationViewController : UINavigationController {
	ChainsTableViewController *chainsTableViewController;
	
@private
	NSManagedObjectContext *managedObjectContext;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) ChainsTableViewController *chainsTableViewController;

@end
