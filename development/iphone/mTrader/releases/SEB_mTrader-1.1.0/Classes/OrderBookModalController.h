//
//  OrderBookModalController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;
@class OrderBookController;

@protocol OrderBookModalControllerDelegate;

@interface OrderBookModalController : UIViewController {
@private
	id <OrderBookModalControllerDelegate> delegate;
	
	OrderBookController *orderBook;
	Symbol *_symbol;
	NSManagedObjectContext *_managedObjectContext;
	
	UILabel *askSizeLabel;
	UILabel *askValueLabel;
	UILabel *bidSizeLabel;
	UILabel *bidValueLabel;
	
}

@property (assign) id <OrderBookModalControllerDelegate> delegate;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
@end

@protocol OrderBookModalControllerDelegate
- (void)orderBookModalControllerDidFinish:(OrderBookModalController *)controller;
@end