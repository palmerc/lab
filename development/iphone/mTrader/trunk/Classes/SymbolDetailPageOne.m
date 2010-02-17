//
//  SymbolDetailPageOne.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


#import <QuartzCore/QuartzCore.h>
#import "SymbolDetailPageOne.h"
#import "Symbol.h"
#import "Feed.h"
#import "SymbolDynamicData.h"

#pragma mark -
#pragma mark SymbolDetailPageOne implementation
@implementation SymbolDetailPageOne
@synthesize symbol;

#pragma mark -
#pragma mark Initialization
- (id)init {
    if (self = [super init]) {	
		mainFont = [UIFont systemFontOfSize:14.0];

		globalY = 0.0;
		
		sectionHeaders = [NSArray arrayWithObjects:@"Symbol Information", @"Trades Information", @"Fundamentals", nil];
		symbolLabels = [NSArray arrayWithObjects:@"type", @"isin", @"currency", @"country", nil];
		tradesLabels = [NSArray arrayWithObjects:@"lastTrade", @"VWAP", @"lastTradeTime", @"open", @"turnover", @"high", @"volume", @"low", nil];
		fundamentalsLabels = [NSArray arrayWithObjects:@"segment", @"marketCapitalization", @"outstandingShares", @"dividend", @"dividendDate", nil];
	}
    return self;
}

#define TEXT_LEFT_MARGIN    8.0
#define TEXT_RIGHT_MARGIN   8.0

#define SECTION_HEADER_HEIGHT 22.0
#define LINE_WIDTH          320.0
#define LINE_HEIGHT         22.0
#define HEIGHT_PADDING		4.0;

#define EDITING_INSET       10.0
#define BUTTON_WIDTH        85.0
#define TIME_WIDTH          102.0
#define DESCRIPTION_WIDTH   200.0

- (void)setHeader:(NSString *)header {
	UIFont *headerFont = [UIFont boldSystemFontOfSize:18.0];
	UIColor *sectionTextColor = [UIColor colorWithWhite:1.0 alpha:1.0];
	UIColor *sectionTextShadowColor = [UIColor colorWithWhite:0.0 alpha:0.44];
	CGSize shadowOffset = CGSizeMake(0.0, 1.0);
	
	// Render the dynamic gradient
	CAGradientLayer *headerGradient = [CAGradientLayer layer];
	UIColor *topLine = [UIColor colorWithRed:111.0/255.0 green:118.0/255.0 blue:123.0/255.0 alpha:1.0];
	UIColor *shine = [UIColor colorWithRed:165.0/255.0 green:177/255.0 blue:186.0/255.0 alpha:1.0];
	UIColor *topOfFade = [UIColor colorWithRed:144.0/255.0 green:159.0/255.0 blue:170.0/255.0 alpha:1.0];
	UIColor *bottomOfFade = [UIColor colorWithRed:184.0/255.0 green:193.0/255.0 blue:200.0/255.0 alpha:1.0];
	UIColor *bottomLine = [UIColor colorWithRed:152.0/255.0 green:158.0/255.0 blue:164.0/255.0 alpha:1.0];
	NSArray *colors = [NSArray arrayWithObjects:(id)topLine.CGColor, (id)shine.CGColor, (id)topOfFade.CGColor, (id)bottomOfFade.CGColor, (id)bottomLine.CGColor, nil];
	NSArray *locations = [NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0], [NSNumber numberWithFloat:0.05],[NSNumber numberWithFloat:0.10],[NSNumber numberWithFloat:0.95],[NSNumber numberWithFloat:1.0],nil];
	headerGradient.colors = colors;
	headerGradient.locations = locations;

	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	
	CGSize headerSize = [header sizeWithFont:headerFont];
	
	CGRect labelFrame = CGRectMake(TEXT_LEFT_MARGIN, globalY, headerSize.width, headerSize.height);
	CGRect viewFrame = CGRectMake(screenBounds.origin.x, globalY, screenBounds.size.width, headerSize.height);
	
	UIView *view = [[UIView alloc] initWithFrame:viewFrame];
	UILabel *label = [[UILabel alloc] initWithFrame:labelFrame];

	[view.layer insertSublayer:headerGradient atIndex:0];
	headerGradient.frame = view.bounds;
	
	label.text = header;
	[label setFont:headerFont];
	[label setTextColor:sectionTextColor];
	[label setShadowColor:sectionTextShadowColor];
	[label setShadowOffset:shadowOffset];
	[label setBackgroundColor:[UIColor clearColor]];
	
	[self addSubview:view];
	[self addSubview:label];
	
	globalY += view.bounds.size.height;
	
	[label release];
	[view release];
}


- (NSString *)createTheStringFromKey:(NSString *)key inObject:(id)object {
	static NSDateFormatter *dateFormatter = nil;
	if (dateFormatter == nil) {
		dateFormatter = [[NSDateFormatter alloc] init];
		[dateFormatter setDateStyle:NSDateFormatterShortStyle];
	}
	
	static NSDateFormatter *timeFormatter = nil;
	if (timeFormatter == nil) {
		timeFormatter = [[NSDateFormatter alloc] init];
		[timeFormatter setDateFormat:@"HH:mm:ss"];
	}
	
	static NSNumberFormatter *doubleFormatter = nil;
	if (doubleFormatter == nil) {
		doubleFormatter = [[NSNumberFormatter alloc] init];
		[doubleFormatter setNumberStyle:NSNumberFormatterDecimalStyle];
		[doubleFormatter setUsesSignificantDigits:YES];
	}
	
	static NSNumberFormatter *integerFormatter = nil;
	if (integerFormatter == nil) {
		integerFormatter = [[NSNumberFormatter alloc] init];
		[integerFormatter setNumberStyle:NSNumberFormatterNoStyle];
	}
	
	static NSNumberFormatter *percentFormatter = nil;
	if (percentFormatter == nil) {
		percentFormatter = [[NSNumberFormatter alloc] init];
		[percentFormatter setNumberStyle:NSNumberFormatterPercentStyle];
		[percentFormatter setUsesSignificantDigits:YES];
	}

	id dataObject = [object valueForKey:key];

	if ([dataObject isKindOfClass:[NSString class]]) {
		return dataObject;
	} else if ([dataObject isKindOfClass:[NSNumber class]]) {
		return [doubleFormatter stringFromNumber:dataObject];
	} else {
		return @"";
	}
}

- (void)renderMe {
	[self setHeader:[sectionHeaders objectAtIndex:0]];
	for (NSString *info in symbolLabels) {
		NSString *data = [self createTheStringFromKey:info inObject:symbol];		
		NSString *label = [NSString stringWithFormat:@"%@: %@", info, data];
		CGRect frame = [self leftSideFrameWithLabel:label];
		UILabel *labelLabel = [[UILabel alloc] initWithFrame:frame];
		labelLabel.text = label;
		[labelLabel setTextColor:[UIColor blackColor]];
		[labelLabel setBackgroundColor:[UIColor clearColor]];
		[labelLabel setFont:mainFont];
		[self addSubview:labelLabel];
		[labelLabel release];
	}
	
	[self setHeader:[sectionHeaders objectAtIndex:1]];
	for (NSString *info in tradesLabels) {		
		NSString *data = [self createTheStringFromKey:info inObject:symbol.symbolDynamicData];		
		NSString *label = [NSString stringWithFormat:@"%@: %@", info, data];
		CGRect frame = [self leftSideFrameWithLabel:label];
		UILabel *labelLabel = [[UILabel alloc] initWithFrame:frame];
		labelLabel.text = label;
		[labelLabel setTextColor:[UIColor blackColor]];
		[labelLabel setBackgroundColor:[UIColor clearColor]];
		[labelLabel setFont:mainFont];
		[self addSubview:labelLabel];
		[labelLabel release];
	}
	
	[self setHeader:[sectionHeaders objectAtIndex:2]];
	for (NSString *info in fundamentalsLabels) {
		NSString *data = [self createTheStringFromKey:info inObject:symbol.symbolDynamicData];
		NSString *label = [NSString stringWithFormat:@"%@: %@", info, data];
		CGRect frame = [self leftSideFrameWithLabel:label];
		UILabel *labelLabel = [[UILabel alloc] initWithFrame:frame];
		labelLabel.text = label;
		[labelLabel setTextColor:[UIColor blackColor]];
		[labelLabel setBackgroundColor:[UIColor clearColor]];
		[labelLabel setFont:mainFont];
		[self addSubview:labelLabel];
		[labelLabel release];
	}
}

- (CGRect)leftSideFrameWithLabel:(NSString *)label {
	CGSize textLabelSize = [label sizeWithFont:mainFont];
	CGFloat x = TEXT_LEFT_MARGIN;
	CGFloat lineHeight = textLabelSize.height;
	CGFloat y = globalY;
	CGFloat width = textLabelSize.width;
	CGFloat height = lineHeight;
	globalY += height;
	return CGRectMake(x, y, width, height);
}

- (CGRect)_rightSideFrameWithLabel:(NSString *)label line:(NSUInteger)line {	
	CGRect bounds = [[UIScreen mainScreen] bounds];
	CGSize textLabelSize = [label sizeWithFont:mainFont];
	CGFloat x = bounds.size.width / 2;
	CGFloat lineHeight = HEIGHT_PADDING + textLabelSize.height;
	CGFloat y = lineHeight * line;
	CGFloat width = textLabelSize.width - TEXT_RIGHT_MARGIN;
	CGFloat height = textLabelSize.height + HEIGHT_PADDING;
	return CGRectMake(x, y, width, height);
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[mainFont release];
	
	[symbolInfoHeaderLabel release];
	[tradesInfoHeaderLabel release];
	[fundamentalsHeaderLabel release];
	
	[symbolInfoHeaderView release];
	[tradesInfoHeaderView release];
	[fundamentalsHeaderView release];
	
	[typeTextLabel release];
	[isinTextLabel release];
	[currencyTextLabel release];
	[countryTextLabel release];
	[lastTradeTextLabel release];
	[lastTradeTimeTextLabel release];
	[vwapTextLabel release];
	[openPriceTextLabel release];
	[tradesTextLabel release];
	[highPriceTextLabel release];
	[lowPriceTextLabel release];
	[volumeTextLabel release];
	
	[tickerSymbolLabel release];
	[descriptionLabel release];
	[typeLabel release];
	[isinLabel release];
	[currencyLabel release];
	[countryLabel release];
	[lastTradeLabel release];
	[vwapLabel release];
	[lastTradeTimeLabel release];
	[openPriceLabel release];
	[highPriceLabel release];
	[lowPriceLabel release];
	[tradesLabel release];
	[volumeLabel release];
    
	[super dealloc];
}

@end
