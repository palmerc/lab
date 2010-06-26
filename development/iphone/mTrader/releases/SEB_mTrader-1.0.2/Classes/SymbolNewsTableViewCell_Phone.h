//
//  SymbolNewsTableViewCell_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@class SymbolNewsRelationship;

@interface SymbolNewsTableViewCell_Phone : UITableViewCell {
@private
	SymbolNewsRelationship *_symbolNewsRelationship;
	
	UILabel *feedLabel;
	UILabel *headlineLabel;
	UILabel *dateTimeLabel;
	
	UIFont *headlineLabelFont;
	UIFont *bottomLineLabelFont;
}

@property (nonatomic, retain) SymbolNewsRelationship *symbolNewsRelationship;

@end
