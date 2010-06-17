//
//  SymbolAddController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;
@class mTraderCommunicator;
@protocol SymbolAddControllerDelegate;

@interface SymbolAddController_Phone : UIViewController {
@private
	CGRect _frame;
	
	id <SymbolAddControllerDelegate> delegate;
}

@property (nonatomic, assign) id <SymbolAddControllerDelegate> delegate;

- (id)initWithFrame:(CGRect)frame;
- (void)changeQFieldsStreaming;

@end

@protocol SymbolAddControllerDelegate
- (void)symbolAddControllerDidFinish:(SymbolAddController_Phone *)controller didAddSymbol:(NSString *)tickerSymbol;
@end