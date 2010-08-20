//
//  ChartController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"
@class Symbol;
@class ChartView;
@protocol ChartControllerDelegate;

@interface ChartController : UIViewController {
@private
	id <ChartControllerDelegate> delegate;
	
	Symbol *_symbol;
	ChartView *_chartView;
	UIToolbar *_periodSelectionToolbar;
	
	NSUInteger _period;
	NSString *_orientation;
}

@property (assign) id <ChartControllerDelegate> delegate;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) NSString *orientation;
@property NSUInteger period;

- (id)initWithSymbol:(Symbol *)symbol;
- (void)requestChartForPeriod:(NSUInteger)period;

@end

@protocol ChartControllerDelegate
- (void)chartControllerDidFinish:(ChartController *)controller;
@end