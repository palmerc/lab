//
//  LastChangeView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "LastChangeView.h"

#import "mTraderCommunicator.h"
#import "ChartController.h"

#import "Feed.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "ChartController.h"

@interface LastChangeView ()
- (void)updateSymbol;
@end

@implementation LastChangeView
@synthesize symbol = _symbol;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self != nil) {
		_symbol = nil;
		
		_lastLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_lastLabel.font = [UIFont boldSystemFontOfSize:44.0];
		_lastLabel.adjustsFontSizeToFitWidth = YES;
		_lastLabel.textAlignment = UITextAlignmentCenter;
		[self addSubview:_lastLabel];
		
		_lastChangeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_lastChangeLabel.font = [UIFont boldSystemFontOfSize:14.0];
		_lastChangeLabel.adjustsFontSizeToFitWidth = YES;
		_lastChangeLabel.textAlignment = UITextAlignmentLeft;
		[self addSubview:_lastChangeLabel];
		
		_lastPercentChangeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_lastPercentChangeLabel.font = [UIFont boldSystemFontOfSize:14.0];
		_lastPercentChangeLabel.adjustsFontSizeToFitWidth = YES;
		_lastPercentChangeLabel.textAlignment = UITextAlignmentRight;
		[self addSubview:_lastPercentChangeLabel];
		
		_timeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_timeLabel.font = [UIFont systemFontOfSize:12.0];
		_timeLabel.textAlignment = UITextAlignmentLeft;
		[self addSubview:_timeLabel];
		
		_chartButton = [[UIButton buttonWithType:UIButtonTypeCustom] retain];
		[self addSubview:_chartButton];
		
		_chartController = [[ChartController alloc] init];
		UIView *chartView = _chartController.view;
	
		[_chartButton addSubview:chartView];
	}
	return self;
}

- (void)layoutSubviews {
	CGRect bounds = self.bounds;
	
	CGSize lastFontSize = [@"X" sizeWithFont:_lastLabel.font];
	CGSize lastChangeFontSize = [@"X" sizeWithFont:_lastChangeLabel.font];
	CGSize lastPercentChangeFontSize = [@"X" sizeWithFont:_lastPercentChangeLabel.font];
	CGSize timeFontSize = [@"X" sizeWithFont:_timeLabel.font];
	
	CGFloat globalY = 0.0f;
	_lastLabel.frame = CGRectMake(0.0f, globalY, bounds.size.width, lastFontSize.height);
	globalY += lastFontSize.height;
	
	_lastChangeLabel.frame = CGRectMake(0.0f, globalY, bounds.size.width / 2.0f, lastChangeFontSize.height);	
	_lastPercentChangeLabel.frame = CGRectMake(0.0f + bounds.size.width / 2.0f, globalY, bounds.size.width / 2.0f, lastPercentChangeFontSize.height);
	globalY += lastPercentChangeFontSize.height;
	
	_timeLabel.frame = CGRectMake(0.0f, globalY, bounds.size.width, timeFontSize.height);
	globalY += timeFontSize.height;
	
	_chartButton.frame = CGRectMake(0.0f, globalY, bounds.size.width, globalY);
}


#pragma mark -
#pragma mark Symbol methods
- (void)setSymbol:(Symbol *)symbol {
	if (_symbol == symbol) {
		return;
	}
	[_symbol release];
	_symbol = [symbol retain];
	
	_chartController.symbol = symbol;
	
	[_symbol addObserver:self forKeyPath:@"symbolDynamicData.lastTrade" options:NSKeyValueObservingOptionNew context:nil];
	[_symbol addObserver:self forKeyPath:@"symbolDynamicData.change" options:NSKeyValueObservingOptionNew context:nil];
	[_symbol addObserver:self forKeyPath:@"symbolDynamicData.changePercent" options:NSKeyValueObservingOptionNew context:nil];
	
	[self updateSymbol];
}

- (void)updateSymbol {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
	}
	
	static NSNumberFormatter *percentFormatter = nil;
	if (percentFormatter == nil) {
		percentFormatter = [[NSNumberFormatter alloc] init];
		[percentFormatter setNumberStyle:NSNumberFormatterPercentStyle];
		[percentFormatter setUsesSignificantDigits:YES];
	}
	
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	}
	
	// Red Font for Down, Blue Font for Up, Black for No Change
	UIColor *textColor = nil;
	
	NSComparisonResult comparison = [_symbol.symbolDynamicData.change compare:[NSNumber numberWithDouble:0.0]];
	switch (comparison) {
		case NSOrderedAscending:
			textColor = [UIColor redColor];
			break;
		case NSOrderedDescending:
			textColor = [UIColor blueColor];
			break;
		case NSOrderedSame:
			textColor = [UIColor blackColor];
			break;
		default:
			textColor = [UIColor blackColor];
			break;
	}
	
	[_lastChangeLabel setTextColor:textColor];
	[_lastPercentChangeLabel setTextColor:textColor];
	
	NSUInteger decimals = [_symbol.feed.decimals integerValue];
	[doubleFormatter setMinimumFractionDigits:decimals];
	[doubleFormatter setMaximumFractionDigits:decimals];
	
	_lastLabel.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.lastTrade];
	_lastChangeLabel.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.change];
	_lastPercentChangeLabel.text = [percentFormatter stringFromNumber:_symbol.symbolDynamicData.changePercent];
	_timeLabel.text = [dateFormatter stringFromDate:_symbol.symbolDynamicData.lastTradeTime];
}

#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"symbolDynamicData.lastTrade"] || 
		[keyPath isEqualToString:@"symbolDynamicData.change"] || 
		[keyPath isEqualToString:@"symbolDynamicData.changePercent"]) {
		[self updateSymbol];
	}
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_symbol removeObserver:self forKeyPath:@"symbolDynamicData.lastTrade"];
	[_symbol removeObserver:self forKeyPath:@"symbolDynamicData.change"];
	[_symbol removeObserver:self forKeyPath:@"symbolDynamicData.changePercent"];
	
	[_symbol release];
	[_timeLabel release];
	[_lastLabel release];
	[_lastChangeLabel release];
	[_lastPercentChangeLabel release];
	[_chartButton release];
	[_chartController release];

	[super dealloc];
}

@end
