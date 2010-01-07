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
@protocol StockSearchControllerDelegate;

@interface StockSearchController : UIViewController <UISearchBarDelegate, StockAddDelegate> {
	id <StockSearchControllerDelegate> delegate;
	iTraderCommunicator *communicator;
	IBOutlet UISearchBar *_searchBar;
	
	NSString *tickerSymbol;
}

@property (assign) id <StockSearchControllerDelegate> delegate;
@property (nonatomic, retain) IBOutlet UISearchBar *searchBar;
@property (nonatomic, retain) NSString *tickerSymbol;
@end

@protocol StockSearchControllerDelegate
- (void)stockSearchControllerDidFinish:(StockSearchController *)controller didAddSymbol:(NSString *)tickerSymbol;
@end