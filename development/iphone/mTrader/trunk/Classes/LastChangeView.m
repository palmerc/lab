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
#import "Chart.h"


@implementation LastChangeView
@synthesize viewController = _viewController;
@synthesize symbol = _symbol;
@synthesize time = _time;
@synthesize last = _last;
@synthesize lastChange = _lastChange;
@synthesize lastPercentChange = _lastPercentChange;
@synthesize chart = _chart;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self != nil) {
		_last = [[UILabel alloc] initWithFrame:CGRectZero];
		_last.font = [UIFont boldSystemFontOfSize:44.0];
		_last.adjustsFontSizeToFitWidth = YES;
		_last.textAlignment = UITextAlignmentCenter;
		[self addSubview:_last];
		
		_lastChange = [[UILabel alloc] initWithFrame:CGRectZero];
		_lastChange.font = [UIFont boldSystemFontOfSize:14.0];
		_lastChange.adjustsFontSizeToFitWidth = YES;
		_lastChange.textAlignment = UITextAlignmentLeft;
		[self addSubview:_lastChange];
		
		_lastPercentChange = [[UILabel alloc] initWithFrame:CGRectZero];
		_lastPercentChange.font = [UIFont boldSystemFontOfSize:14.0];
		_lastPercentChange.adjustsFontSizeToFitWidth = YES;
		_lastPercentChange.textAlignment = UITextAlignmentRight;
		[self addSubview:_lastPercentChange];
		
		_time = [[UILabel alloc] initWithFrame:CGRectZero];
		_time.font = [UIFont systemFontOfSize:12.0];
		_time.textAlignment = UITextAlignmentLeft;
		[self addSubview:_time];
		
		_chart = [[UIButton alloc] initWithFrame:CGRectZero];
		[_chart addTarget:_viewController action:@selector(chart:) forControlEvents:UIControlEventTouchUpInside];
		_chart.backgroundColor = [UIColor clearColor];
		[self addSubview:_chart];
	}
	return self;
}

- (void)layoutSubviews {
	CGRect bounds = self.bounds;
	
	CGSize lastFontSize = [@"X" sizeWithFont:_last.font];
	CGSize lastChangeFontSize = [@"X" sizeWithFont:_lastChange.font];
	CGSize lastPercentChangeFontSize = [@"X" sizeWithFont:_lastPercentChange.font];
	CGSize timeFontSize = [@"X" sizeWithFont:_time.font];
	
	CGFloat globalY = 0.0f;
	_last.frame = CGRectMake(0.0f, globalY, bounds.size.width, lastFontSize.height);
	globalY += lastFontSize.height;
	
	_lastChange.frame = CGRectMake(0.0f, globalY, bounds.size.width / 2.0f, lastChangeFontSize.height);	
	_lastPercentChange.frame = CGRectMake(0.0f + bounds.size.width / 2.0f, globalY, bounds.size.width / 2.0f, lastPercentChangeFontSize.height);
	globalY += lastPercentChangeFontSize.height;
	
	_time.frame = CGRectMake(0.0f, globalY, bounds.size.width, timeFontSize.height);
	globalY += timeFontSize.height;
	
	_chart.frame = CGRectMake(0.0f, globalY, bounds.size.width, globalY);
	
	CGFloat chartWidth = floorf(bounds.size.width) - 1.0f;
	CGFloat chartHeight = floorf(globalY) - 1.0f;
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", _symbol.feed.feedNumber, _symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:365 width:chartWidth height:chartHeight orientation:@"A"];
}


#pragma mark -
#pragma mark Symbol methods
- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	
	[_symbol addObserver:self forKeyPath:@"symbolDynamicData.lastTrade" options:NSKeyValueObservingOptionNew context:nil];
	[_symbol addObserver:self forKeyPath:@"symbolDynamicData.change" options:NSKeyValueObservingOptionNew context:nil];
	[_symbol addObserver:self forKeyPath:@"symbolDynamicData.changePercent" options:NSKeyValueObservingOptionNew context:nil];
	[_symbol addObserver:self forKeyPath:@"chart.data" options:NSKeyValueObservingOptionNew context:nil];
	
	[self updateSymbol];
	[self setNeedsDisplay];
}

- (void)updateChart {
	NSData *data = _symbol.chart.data;
	UIImage *image = [UIImage imageWithData:data];
	[_chart setImage:image forState:UIControlStateNormal];
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
	
	[_lastChange setTextColor:textColor];
	[_lastPercentChange setTextColor:textColor];
	
	NSUInteger decimals = [_symbol.feed.decimals integerValue];
	[doubleFormatter setMinimumFractionDigits:decimals];
	[doubleFormatter setMaximumFractionDigits:decimals];
	
	_last.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.lastTrade];
	_lastChange.text = [doubleFormatter stringFromNumber:_symbol.symbolDynamicData.change];
	_lastPercentChange.text = [percentFormatter stringFromNumber:_symbol.symbolDynamicData.changePercent];
	_time.text = [dateFormatter stringFromDate:_symbol.symbolDynamicData.lastTradeTime];
	
	UIImage *image = [UIImage imageWithData:_symbol.chart.data];
	[_chart setImage:image forState:UIControlStateNormal];
}

#pragma mark -
#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
	if ([keyPath isEqualToString:@"symbolDynamicData.lastTrade"] || 
		[keyPath isEqualToString:@"symbolDynamicData.change"] || 
		[keyPath isEqualToString:@"symbolDynamicData.changePercent"]) {
		[self updateSymbol];
	} else if ([keyPath isEqualToString:@"chart.data"]) {
		[self updateChart];
	}
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_symbol removeObserver:self forKeyPath:@"symbolDynamicData.lastTrade"];
	[_symbol removeObserver:self forKeyPath:@"symbolDynamicData.change"];
	[_symbol removeObserver:self forKeyPath:@"symbolDynamicData.changePercent"];
	[_symbol removeObserver:self forKeyPath:@"chart.data"];
	
	[_symbol release];
	[_time release];
	[_last release];
	[_lastChange release];
	[_lastPercentChange release];
	[_chart release];

	[super dealloc];
}

@end
