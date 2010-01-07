//
//  StockDetailController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 07.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
@class Symbol;

@interface StockDetailController : UIViewController {
	Symbol *_symbol;
	
	IBOutlet UILabel *stockNameLabel;
	IBOutlet UILabel *stockISINLabel;
	IBOutlet UILabel *exchangeLabel;
	IBOutlet UILabel *lastChangeLabel;
	IBOutlet UILabel *percentChangeLabel;
	IBOutlet UILabel *tickerSymbolLabel;
	IBOutlet UILabel *lowLabel;
	IBOutlet UILabel *highLabel;
	IBOutlet UILabel *volumeLabel;
	IBOutlet UILabel *openLabel;
	
	IBOutlet UIImageView *graphImage;
}

@property (nonatomic, retain) Symbol *symbol;
@property (nonatomic, retain) IBOutlet UILabel *stockNameLabel;
@property (nonatomic, retain) IBOutlet UILabel *stockISINLabel;
@property (nonatomic, retain) IBOutlet UILabel *exchangeLabel;
@property (nonatomic, retain) IBOutlet UILabel *lastChangeLabel;
@property (nonatomic, retain) IBOutlet UILabel *percentChangeLabel;
@property (nonatomic, retain) IBOutlet UILabel *tickerSymbolLabel;
@property (nonatomic, retain) IBOutlet UILabel *lowLabel;
@property (nonatomic, retain) IBOutlet UILabel *highLabel;
@property (nonatomic, retain) IBOutlet UILabel *volumeLabel;
@property (nonatomic, retain) IBOutlet UILabel *openLabel;

@property (nonatomic, retain) IBOutlet UIImageView *graphImage;

- (id)initWithSymbol:(Symbol *)symbol;

@end
