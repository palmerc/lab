//
//  OrderBookController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
@protocol OrderBookControllerDelegate;
@class Symbol;

@interface OrderBookController : UIViewController <NSFetchedResultsControllerDelegate, SymbolsDataDelegate, UITableViewDelegate, UITableViewDataSource> {
@private
	NSManagedObjectContext *_managedObjectContext;
	NSFetchedResultsController *_fetchedResultsController;

	Symbol *_symbol;
	NSArray *_bidAsks;	
	
	id <OrderBookControllerDelegate> delegate;
	
	UITableView *table;
	
	UIView *askSizeLabel;
	UIView *askValueLabel;
	UIView *bidSizeLabel;
	UIView *bidValueLabel;
}

@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSArray *bidAsks;
@property (assign) id <OrderBookControllerDelegate> delegate;
@property (nonatomic, retain) UITableView *table;

- (id)initWithManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;
- (void)updateSymbol;
- (void)configureCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;
- (UIView *)setHeader:(NSString *)header withFrame:(CGRect)frame;
- (NSArray *)fetchBidAsksForSymbol:(NSString *)tickerSymbol withFeed:(NSString *)mCode;

@end

//@protocol OrderBookControllerDelegate
//- (void)orderBookControllerDidFinish:(OrderBookController *)controller;
//@end
