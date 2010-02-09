//
//  SymbolAddController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;
@class mTraderCommunicator;
@protocol SymbolAddControllerDelegate;

@interface SymbolAddController : UIViewController <UIPickerViewDataSource, UIPickerViewDelegate, UITextFieldDelegate, NSFetchedResultsControllerDelegate> {
	id <SymbolAddControllerDelegate> delegate;
	
	NSFetchedResultsController *fetchedResultsController;
	NSManagedObjectContext *managedObjectContext;

	mTraderCommunicator *communicator;
	UITextField *_tickerField;
	UIButton *_submitButton;
	UIPickerView *_exchangePicker;
	NSString *tickerSymbol;
	NSString *mCode;
}

@property (assign) id <SymbolAddControllerDelegate> delegate;
@property (nonatomic, retain) NSFetchedResultsController *fetchedResultsController;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) IBOutlet UITextField *tickerField;
@property (nonatomic, retain) IBOutlet UIButton *submitButton;
@property (nonatomic, retain) IBOutlet UIPickerView *exchangePicker;
@property (nonatomic, retain) NSString *tickerSymbol;
@property (nonatomic, retain) NSString *mCode;

-(IBAction) submit:(id)sender;
@end

@protocol SymbolAddControllerDelegate
- (void)symbolAddControllerDidFinish:(SymbolAddController *)controller didAddSymbol:(NSString *)tickerSymbol;
@end