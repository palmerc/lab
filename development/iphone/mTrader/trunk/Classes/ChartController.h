//
//  ChartController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
@class Symbol;
@protocol ChartControllerDelegate;

@interface ChartController : UIViewController <SymbolsDataDelegate> {
@private
	id <ChartControllerDelegate> delegate;
	NSManagedObjectContext *_managedObjectContext;
		
	Symbol *_symbol;
	UIImageView *_chart;
	UIToolbar *_toolBar;
		
	NSUInteger period;
	CGFloat globalY;

}

@property (assign) id <ChartControllerDelegate> delegate;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UIImageView *chart;
@property (nonatomic, retain) UIToolbar *toolBar;

- (id)initWithSymbol:(Symbol *)symbol;
- (void)updateChart;

- (Symbol *)fetchSymbol:(NSString *)tickerSymbol withFeedNumber:(NSNumber *)feedNumber;

@end

@protocol ChartControllerDelegate
- (void)chartControllerDidFinish:(ChartController *)controller;
@end