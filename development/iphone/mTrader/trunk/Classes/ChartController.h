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

@interface ChartController : UIViewController {
@private
	id <ChartControllerDelegate> delegate;
		
	Symbol *_symbol;
	UIImageView *_chart;
	UIToolbar *_toolBar;
		
	NSUInteger period;
	CGFloat globalY;

}

@property (assign) id <ChartControllerDelegate> delegate;
@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UIImageView *chart;
@property (nonatomic, retain) UIToolbar *toolBar;

- (id)initWithSymbol:(Symbol *)symbol;
- (void)updateChart;

@end

@protocol ChartControllerDelegate
- (void)chartControllerDidFinish:(ChartController *)controller;
@end