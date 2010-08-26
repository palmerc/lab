//
//  ChartController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

#import "ChartView.h"

@class Symbol;
@class ChartView;
@protocol ChartControllerDelegate;

@interface ChartController : UIViewController <ChartRequestDelegate> {
@private
	id <ChartControllerDelegate> delegate;
	
	Symbol *_symbol;
	ChartView *_chartView;
	UIToolbar *_periodSelectionToolbar;
	
	NSUInteger _period;
	NSString *_orientation;
	
	BOOL _modal;
}

@property (assign) id <ChartControllerDelegate> delegate;
@property (nonatomic, retain) Symbol *symbol;
@property NSUInteger period;
@property (nonatomic, retain) NSString *orientation;
@property BOOL modal;

@end

@protocol ChartControllerDelegate
- (void)chartControllerDidFinish:(ChartController *)controller;
@end