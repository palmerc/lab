//
//  OrderBookView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 11.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "OrderBookView.h"
#import "OrderBookController.h"
#import "Symbol.h"

@implementation OrderBookView
@synthesize symbol = _symbol;

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
		_symbol = nil;
		
		orderBookController = [[OrderBookController alloc] init];
		[self addSubview:orderBookController.view];
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
	CGFloat maxHeight = rect.size.height - leftPadding * 2.0f;

	CGRect tableFrame = CGRectMake(leftPadding, leftPadding, maxWidth, maxHeight);
	
	orderBookController.table.frame = tableFrame;
	
}

- (void)dealloc {
	[_symbol release];
	
	[orderBookController release];

    [super dealloc];
}


@end
