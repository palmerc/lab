//
//  TableViewController.h
//  TableDemo
//
//  Created by Cameron Lowell Palmer on 07.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface TableViewController : UITableViewController <UITableViewDataSource, UITableViewDelegate> {
	NSMutableArray *regionsArray;
	NSMutableDictionary *timeZoneDictionary;
}

@end
