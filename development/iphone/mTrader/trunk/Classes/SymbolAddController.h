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

@interface SymbolAddController : UIViewController <UISearchDisplayDelegate, UITableViewDataSource, UITableViewDelegate> {
	id <SymbolAddControllerDelegate> delegate;

	mTraderCommunicator *communicator;
	
	UISearchDisplayController *searchController;
}

@property (nonatomic, assign) id <SymbolAddControllerDelegate> delegate;

- (void)changeQFieldsStreaming;

@end


@protocol SymbolAddControllerDelegate
- (void)symbolAddControllerDidFinish:(SymbolAddController *)controller didAddSymbol:(NSString *)tickerSymbol;
@end