//
//  StockSearchController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
@class Symbol;
@protocol StockSearchControllerDelegate;

@interface StockSearchController : UIViewController <UISearchBarDelegate> {
	id <StockSearchControllerDelegate> delegate;
	
	IBOutlet UISearchBar *searchBar;
}

@property (assign) id <StockSearchControllerDelegate> delegate;
@property (nonatomic, retain) IBOutlet UISearchBar *searchBar;
@end

@protocol StockSearchControllerDelegate
- (void)stockSearchControllerDidFinish:(StockSearchController *)controller didAddSymbol:(Symbol *)symbol;
@end