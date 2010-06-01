//
//  TradesModalController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "mTraderCommunicator.h"

#import "SymbolDataController.h"

@protocol TradesControllerDelegate;

@class Symbol;
@class TradesCell;
@class TradesController;

@interface TradesModalController : UIViewController {
@private
	id <TradesControllerDelegate> delegate;
	
	TradesController *_tradesController;
	NSManagedObjectContext *_managedObjectContext;
	
	Symbol *_symbol;
}

@property (assign) id <TradesControllerDelegate> delegate;
@property (nonatomic, retain) TradesController *tradesController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@end


@protocol TradesControllerDelegate
- (void)tradesControllerDidFinish:(TradesModalController *)controller;
@end

