//
//  OrderBookView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "OrderBookView.h"
#import "OrderBookController.h"
#import "OrderBookTableCellP.h"

#import "Symbol.h"

@implementation OrderBookView
@synthesize symbol = _symbol;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
    if (self = [super initWithFrame:frame]) {
		_symbol = nil;
		
		orderBookController = [[OrderBookController alloc] initWithManagedObjectContext:managedObjectContext];
		[self addSubview:orderBookController.view];
    }
    return self;
}

#pragma mark -
#pragma mark UIView drawing
- (void)drawRect:(CGRect)rect {
  	super.padding = 6.0f;
	super.cornerRadius = 10.0f;
	super.strokeWidth = 0.75f;
	[super drawRect:rect];
	
	CGFloat leftPadding = self.padding + super.strokeWidth + kBlur;
	CGFloat maxWidth = rect.size.width - leftPadding * 2.0f;
	CGFloat maxHeight = rect.size.height - leftPadding * 2.0f;

	CGRect tableFrame = CGRectMake(leftPadding, leftPadding, maxWidth, maxHeight);
	
	orderBookController.table.frame = tableFrame;
}

- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	
	orderBookController.symbol = _symbol;
}

#pragma mark -
#pragma mark Debugging methods
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@ in OrderBookController", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}

#pragma mark -
#pragma mark Memory management
- (void)dealloc {	
	[_symbol release];
	
	[orderBookController release];

    [super dealloc];
}

@end
