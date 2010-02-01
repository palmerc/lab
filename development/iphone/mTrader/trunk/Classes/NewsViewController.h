//
//  NewsViewController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 23.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "mTraderCommunicator.h"

@interface NewsViewController : UITableViewController <mTraderServerDataDelegate> {
	id previousmTraderServerDataDelegate;
	iTraderCommunicator *communicator;
	
	NSMutableArray *_newsArray;
}
@property (nonatomic, assign) iTraderCommunicator *communicator;
@property (nonatomic, assign) id previousmTraderServerDataDelegate;

@property (nonatomic, retain) NSMutableArray *newsArray;

-(void) refreshNews;
@end
