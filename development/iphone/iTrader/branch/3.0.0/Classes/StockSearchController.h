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

@interface StockSearchController : UIViewController <UIPickerViewDataSource, UIPickerViewDelegate, UITextFieldDelegate> {
	id <StockSearchControllerDelegate> delegate;
	id <UIPickerViewDelegate> pickerDelegate;
	iTraderCommunicator *communicator;
	SymbolsController *controller;
	UITextField *_tickerField;
	UIButton *_submitButton;
	UIPickerView *_exchangePicker;
	NSString *tickerSymbol;
	NSString *mCode;
}

@property (assign) id <StockSearchControllerDelegate> delegate;
@property (nonatomic, retain) IBOutlet UITextField *tickerField;
@property (nonatomic, retain) IBOutlet UIButton *submitButton;
@property (nonatomic, retain) IBOutlet UIPickerView *exchangePicker;
@property (nonatomic, retain) NSString *tickerSymbol;
@property (nonatomic, retain) NSString *mCode;

-(IBAction) submit:(id)sender;
@end

@protocol StockSearchControllerDelegate
- (void)stockSearchControllerDidFinish:(StockSearchController *)controller didAddSymbol:(NSString *)tickerSymbol;
@end