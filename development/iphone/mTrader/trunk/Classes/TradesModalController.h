//
//  TradesModalController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "mTraderCommunicator.h"

#import "DataController.h"

@protocol TradesControllerDelegate;

@class Symbol;
@class TradesCell;
@class PastTradesController;

@interface TradesModalController : UIViewController {
@private
	id <TradesControllerDelegate> delegate;
	
	PastTradesController *_tradesController;
	NSManagedObjectContext *_managedObjectContext;
	
	Symbol *_symbol;
}

@property (assign) id <TradesControllerDelegate> delegate;
@property (nonatomic, retain) PastTradesController *tradesController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@end


@protocol TradesControllerDelegate
- (void)tradesControllerDidFinish:(TradesModalController *)controller;
@end

