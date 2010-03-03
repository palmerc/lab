//
//  SymbolNewsItemController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 03.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import "mTraderCommunicator.h"

@class SymbolNewsItemView;

@interface SymbolNewsItemController : UIViewController <SymbolsDataDelegate> {
@private
	NSString *_feedArticle;
	
	SymbolNewsItemView *newsItemView;
}

@property (nonatomic, retain) NSString *feedArticle;

@end
