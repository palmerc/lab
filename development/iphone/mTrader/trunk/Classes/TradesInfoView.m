//
//  TradesInfoView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 28.05.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "TradesInfoView.h"

#import "Symbol.h"

#import "TradesController.h"

@implementation TradesInfoView

@synthesize symbol = _symbol;
@synthesize tradesController = _tradesController; 
@synthesize viewController = _viewController;
@synthesize tradesButton = _tradesButton;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
    if (self = [super initWithFrame:frame]) {
		_symbol = nil;
		
		_tradesController = [[TradesController alloc] initWithManagedObjectContext:managedObjectContext];
		[self addSubview:_tradesController.view];
		
		_viewController = nil;
		_tradesButton = nil;
    }
    return self;
}

#pragma mark -
#pragma mark UIView drawing
- (void)drawRect:(CGRect)rect {
	
  	super.padding = 6.0f;
	super.cornerRadius = 10.0f;
	super.strokeWidth = 0.75f;
		
	CGFloat leftPadding = floorf(super.padding + super.strokeWidth * 2.0 + kBlur);
	CGFloat maxWidth = rect.size.width - leftPadding * 2.0f;
	CGFloat maxHeight = rect.size.height - leftPadding * 2.0f;
	
	
	CGRect tableFrame = CGRectMake(leftPadding, leftPadding, maxWidth, maxHeight);
	
	_tradesButton = [[UIButton alloc] initWithFrame:rect];
	[_tradesButton addTarget:self.viewController action:@selector(trades:) forControlEvents:UIControlEventTouchUpInside];
		
	_tradesController.view.frame = tableFrame;
	
	[self addSubview:_tradesButton];
	
	[super drawRect:rect];
}

- (void)setSymbol:(Symbol *)symbol {
	_symbol = [symbol retain];
	_tradesController.symbol = _symbol;
}


#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[_tradesButton release];
	
	[_symbol release];
	
	[_viewController release];
	[_tradesController release];
	
    [super dealloc];
}

@end
