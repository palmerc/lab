//
//  SymbolDetailPageOne.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class Symbol;

@interface SymbolDetailPageOne : UIView {
	Symbol *symbol;
	
@private
	UIFont *mainFont;
	
	NSArray *sectionHeaders;
	NSArray *symbolLabels;
	NSArray *tradesLabels;
	NSArray *fundamentalsLabels;
	
	CGFloat globalY;
		
	UIView *symbolInfoHeaderView;
	UILabel *symbolInfoHeaderLabel;
	
	UIView *tradesInfoHeaderView;
	UILabel *tradesInfoHeaderLabel;
	
	UIView *fundamentalsHeaderView;
	UILabel *fundamentalsHeaderLabel;
	
	UILabel *typeTextLabel;
	UILabel *isinTextLabel;
	UILabel *currencyTextLabel;
	UILabel *countryTextLabel;
	UILabel *lastTradeTextLabel;
	UILabel *lastTradeTimeTextLabel;
	UILabel *vwapTextLabel;
	UILabel *openPriceTextLabel;
	UILabel *tradesTextLabel;
	UILabel *highPriceTextLabel;
	UILabel *lowPriceTextLabel;
	UILabel *volumeTextLabel;
	
	UILabel *tickerSymbolLabel;
	UILabel *descriptionLabel;
	UILabel *typeLabel;
	UILabel *isinLabel;
	UILabel *currencyLabel;
	UILabel *countryLabel;
	UILabel *lastTradeLabel;
	UILabel *vwapLabel;
	UILabel *lastTradeTimeLabel;
	UILabel *openPriceLabel;
	UILabel *highPriceLabel;
	UILabel *lowPriceLabel;
	UILabel *tradesLabel;
	UILabel *volumeLabel;
	CGSize tickerLabelSize;
}

@property (nonatomic, retain) Symbol *symbol;

- (NSString *)createTheStringFromKey:(NSString *)key inObject:(id)object;
- (CGRect)leftSideFrameWithLabel:(NSString *)label;
- (void)renderMe;

@end
