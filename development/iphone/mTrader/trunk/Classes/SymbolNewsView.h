//
//  SymbolNewsView.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "RoundedRectangle.h"

@class Symbol;
@class SymbolNewsController_Phone;

@interface SymbolNewsView : RoundedRectangle {
	Symbol *_symbol;
	
	SymbolNewsController_Phone *_symbolNewsController;
	UIViewController *_viewController;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) UIViewController *viewController;

- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext;

@end
