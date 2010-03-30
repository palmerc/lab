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
		self.last.font = [UIFont boldSystemFontOfSize:44.0];
		self.last.adjustsFontSizeToFitWidth = YES;
		self.last.textAlignment = UITextAlignmentCenter;
		[self addSubview:self.last];
		
		_lastChange = [[UILabel alloc] initWithFrame:CGRectZero];
		self.lastChange.font = [UIFont boldSystemFontOfSize:14.0];
		self.lastChange.adjustsFontSizeToFitWidth = YES;
		self.lastChange.textAlignment = UITextAlignmentLeft;
		[self addSubview:self.lastChange];
		
		_lastPercentChange = [[UILabel alloc] initWithFrame:CGRectZero];
		self.lastPercentChange.font = [UIFont boldSystemFontOfSize:14.0];
		self.lastPercentChange.adjustsFontSizeToFitWidth = YES;
		self.lastPercentChange.textAlignment = UITextAlignmentRight;
		[self addSubview:self.lastPercentChange];
		
		_time = [[UILabel alloc] initWithFrame:CGRectZero];
		self.time.font = [UIFont systemFontOfSize:12.0];
		self.time.textAlignment = UITextAlignmentLeft;
		[self addSubview:self.time];
		
		_chart = [[UIButton alloc] initWithFrame:CGRectZero];
		[self.chart addTarget:self.viewController action:@selector(chart:) forControlEvents:UIControlEventTouchUpInside];
		self.chart.backgroundColor = [UIColor clearColor];
		[self addSubview:self.chart];
	}
	return self;
}

#pragma mark UIView drawing
- (void)drawRect:(CGRect)rect {
	super.padding = 6.0f;
	super.cornerRadius = 10.0f;
	super.strokeWidth = 0.75f;
	[super drawRect:rect];
	
	CGFloat leftPadding = ceilf(self.padding + super.strokeWidth + kBlur);
	CGFloat maxWidth = rect.size.width - leftPadding * 2.0f;
	
	CGSize lastFontSize = [@"X" sizeWithFont:self.last.font];
	CGSize lastChangeFontSize = [@"X" sizeWithFont:self.lastChange.font];
	CGSize lastPercentChangeFontSize = [@"X" sizeWithFont:self.lastPercentChange.font];
	CGSize timeFontSize = [@"X" sizeWithFont:self.time.font];
	
	CGFloat globalY = leftPadding;
	self.last.frame = CGRectMake(leftPadding, globalY, maxWidth, lastFontSize.height);
	globalY += lastFontSize.height;
	
	self.lastChange.frame = CGRectMake(leftPadding, globalY, maxWidth / 2.0f, lastChangeFontSize.height);	
	self.lastPercentChange.frame = CGRectMake(leftPadding + maxWidth / 2.0f, globalY, maxWidth / 2.0f, lastPercentChangeFontSize.height);
	globalY += lastPercentChangeFontSize.height;
	
	self.time.frame = CGRectMake(leftPadding, globalY, maxWidth, timeFontSize.height);
	globalY += timeFontSize.height;
	
	CGFloat toEdgePadding = self.padding + super.strokeWidth;
	CGFloat maxChartWidth = rect.size.width - toEdgePadding * 2.0f;
	self.chart.frame = CGRectMake(toEdgePadding, globalY, maxChartWidth, globalY);
	chartWidth = lroundf(maxChartWidth) - 1;
	chartHeight = lroundf(globalY) - 1;
	
	NSString *feedTicker = [NSString stringWithFormat:@"%@/%@", [self.symbol.feed.feedNumber stringValue], self.symbol.tickerSymbol];
	[[mTraderCommunicator sharedManager] graphForFeedTicker:feedTicker period:365 width:chartWidth height:chartHeight orientation:@"A"];
}

#pragma mark -
#pragma mark Symbol methods
- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	
	[self.symbol addObserver:self forKeyPath:@"symbolDynamicData.lastTrade" options:NSKeyValueObservingOptionNew context:nil];
	[self.symbol addObserver:self forKeyPath:@"symbolDynamicData.change" options:NSKeyValueObservingOptionNew context:nil];
	[self.symbol addObserver:self forKeyPath:@"symbolDynamicData.changePercent" options:NSKeyValueObservingOptionNew context:nil];
	[self.symbol addObserver:self forKeyPath:@"chart.data" options:NSKeyValueObservingOptionNew context:nil];
	
	[self updateSymbol];
	[self setNeedsDisplay];
}

- (void)updateChart {
	NSData *data = self.symbol.chart.data;
	UIImage *image = [UIImage imageWithData:data];
	[self.chart setImage:image forState:UIControlStateNormal];
}

- (void)updateSymbol {
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
	
	NSComparisonResult comparison = [self.symbol.symbolDynamicData.change compare:[NSNumber numberWithDouble:0.0]];
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
	
	[self.lastChange setTextColor:textColor];
	[self.lastPercentChange setTextColor:textColor];
	
	NSUInteger decimals = [self.symbol.feed.decimals integerValue];
	[doubleFormatter setMinimumFractionDigits:decimals];
	[doubleFormatter setMaximumFractionDigits:decimals];
	
	self.last.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTrade];
	self.lastChange.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.change];
	self.lastPercentChange.text = [percentFormatter stringFromNumber:self.symbol.symbolDynamicData.changePercent];
	self.time.text = self.symbol.symbolDynamicData.lastTradeTime;
	
	UIImage *image = [UIImage imageWithData:self.symbol.chart.data];
	[self.chart setImage:image forState:UIControlStateNormal];
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
	[self.symbol removeObserver:self forKeyPath:@"symbolDynamicData.lastTrade"];
	[self.symbol removeObserver:self forKeyPath:@"symbolDynamicData.change"];
	[self.symbol removeObserver:self forKeyPath:@"symbolDynamicData.changePercent"];
	[self.symbol removeObserver:self forKeyPath:@"chart.data"];
	
	[_symbol release];
	[_time release];
	[_last release];
	[_lastChange release];
	[_lastPercentChange release];
	[_chart release];

	[super dealloc];
}

@end
