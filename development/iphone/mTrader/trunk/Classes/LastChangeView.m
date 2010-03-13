//
//  LastChangeView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 09.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "LastChangeView.h"
#import "Symbol.h"
#import "SymbolDynamicData.h"
#import "Chart.h"


@implementation LastChangeView
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
		self.lastChange.font = [UIFont systemFontOfSize:14.0];
		self.lastChange.adjustsFontSizeToFitWidth = YES;
		self.lastChange.textAlignment = UITextAlignmentLeft;
		[self addSubview:self.lastChange];
		
		_lastPercentChange = [[UILabel alloc] initWithFrame:CGRectZero];
		self.lastPercentChange.font = [UIFont systemFontOfSize:14.0];
		self.lastPercentChange.adjustsFontSizeToFitWidth = YES;
		self.lastPercentChange.textAlignment = UITextAlignmentRight;
		[self addSubview:self.lastPercentChange];
		
		_time = [[UILabel alloc] initWithFrame:CGRectZero];
		self.time.font = [UIFont systemFontOfSize:12.0];
		self.time.textAlignment = UITextAlignmentLeft;
		[self addSubview:self.time];
		
		_chart = [[UIImageView alloc] initWithFrame:CGRectZero];
		self.chart.backgroundColor = [UIColor lightGrayColor];
		[self addSubview:self.chart];
	}
	return self;
}

- (void)drawRect:(CGRect)rect {
	super.padding = 6.0f;
	super.cornerRadius = 10.0f;
	super.strokeWidth = 0.75f;
	[super drawRect:rect];
	
	CGFloat leftPadding = self.padding + super.strokeWidth + kBlur;
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
}

- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
	}
	
	static NSDateFormatter *timeFormatter = nil;
	if (timeFormatter == nil) {
		timeFormatter = [[NSDateFormatter alloc] init];
		[timeFormatter setDateFormat:@"HH:mm:ss"];
	}
	
	self.last.text = [doubleFormatter stringFromNumber:self.symbol.symbolDynamicData.lastTrade];
	self.lastChange.text = [self.symbol.symbolDynamicData.lastTradeChange stringValue];
	self.lastPercentChange.text = [self.symbol.symbolDynamicData.lastTradePercentChange stringValue];
	//self.time.text = [timeFormatter stringFromDate:self.symbol.symbolDynamicData.lastTradeTime];
	self.time.text = @"12:00:00";
	
	UIImage *image = [UIImage imageWithData:self.symbol.chart.data];
	self.chart.image = image;
	
	[self setNeedsLayout];

}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_symbol release];
	[_time release];
	[_last release];
	[_lastChange release];
	[_lastPercentChange release];
	[_chart release];

	[super dealloc];
}

@end
