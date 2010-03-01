//
//  SymbolNewsController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 01.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@protocol SymbolNewsDelegate;

@interface SymbolNewsController : UIViewController {
	id <SymbolNewsDelegate> delegate;
}

@property (assign) id <SymbolNewsDelegate> delegate;
- (id)initWithSymbol:(Symbol *)symbol;
@end

@protocol SymbolNewsDelegate
@end