//
//  StockSearchController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "iTraderCommunicator.h"
@class Symbol;
@class iTraderCommunicator;
@class SymbolsController;
@protocol StockSearchControllerDelegate;

@interface StockSearchController : UIViewController <UIPickerViewDataSource, UIPickerViewDelegate, StockAddDelegate> {
	id <StockSearchControllerDelegate> delegate;
	iTraderCommunicator *communicator;
	SymbolsController *controller;
	UITextField *_tickerField;
	UIButton *_submitButton;
	UIPickerView *_exchangePicker;
	NSString *tickerSymbol;
}

@property (assign) id <StockSearchControllerDelegate> delegate;
@property (nonatomic, retain) IBOutlet UITextField *tickerField;
@property (nonatomic, retain) IBOutlet UIButton *submitButton;
@property (nonatomic, retain) IBOutlet UIPickerView *exchangePicker;
@property (nonatomic, retain) NSString *tickerSymbol;

-(IBAction) submit:(id)sender;
@end

@protocol StockSearchControllerDelegate
- (void)stockSearchControllerDidFinish:(StockSearchController *)controller didAddSymbol:(NSString *)tickerSymbol;
@end