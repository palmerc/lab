//
//  SymbolSearchController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.06.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "DataController.h"

@class mTraderCommunicator;

@interface SymbolSearchController_Phone : UITableViewController <SearchResultsDelegate, UISearchDisplayDelegate, UISearchBarDelegate> {
@private	
	mTraderCommunicator *communicator;
	
	NSArray *_searchResults;
}

@property (nonatomic, retain) NSArray *searchResults;

@end
