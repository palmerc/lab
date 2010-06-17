//
//  SymbolSearchController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 15.06.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "DataController.h"

#import "SymbolSearchCoveringView_Phone.h"

@class mTraderCommunicator;

@interface SymbolSearchController_Phone : UITableViewController <UISearchBarDelegate, SearchResultsDelegate, CoveringViewDelegate> {
@private	
	mTraderCommunicator *communicator;
	
	SymbolSearchCoveringView_Phone *_coveringView;
	UISearchBar *_searchBar;
	
	NSMutableArray *_searchResults;
}

@property (assign) UISearchBar *searchBar;
@property (nonatomic, retain) NSMutableArray *searchResults;

@end
