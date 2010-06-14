//
//  SymbolAddController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.01.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "DataController.h"

@class Symbol;
@class mTraderCommunicator;
@protocol SymbolAddControllerDelegate;

@interface SymbolAddController_Phone : UITableViewController <UISearchBarDelegate, SearchResultsDelegate> {
@private
	CGRect _frame;
	id <SymbolAddControllerDelegate> delegate;
	NSManagedObjectContext *_managedObjectContext;
	
	mTraderCommunicator *communicator;
	
	//UISearchDisplayController *searchController;
	
	NSArray *_searchResults;
}

@property (nonatomic, assign) id <SymbolAddControllerDelegate> delegate;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSArray *searchResults;

- (id)initWithFrame:(CGRect)frame;
- (void)changeQFieldsStreaming;

@end


@protocol SymbolAddControllerDelegate
- (void)symbolAddControllerDidFinish:(SymbolAddController_Phone *)controller didAddSymbol:(NSString *)tickerSymbol;
@end