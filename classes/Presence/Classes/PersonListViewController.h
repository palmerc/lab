//
//  PersonListViewController.h
//  Presence
//
//  Created by Cameron Lowell Palmer on 09.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
@class Person;

@protocol PersonDetailDelegate <NSObject>
- (void)personWasSelected:(Person *)person;
@end


@interface PersonListViewController : UITableViewController <UITableViewDataSource, UITableViewDelegate> {
	NSMutableArray *people;
}

@end
