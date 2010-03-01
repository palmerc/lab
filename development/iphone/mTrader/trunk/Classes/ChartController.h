//
//  ChartController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@protocol ChartDelegate;

@interface ChartController : UIViewController {
	id <ChartDelegate> delegate;
}

@property (assign) id <ChartDelegate> delegate;

- (id)initWithSymbol:(Symbol *)symbol;
@end

@protocol ChartDelegate
@end