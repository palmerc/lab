//
//  SymbolNewsView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "SymbolNewsView.h"

#import "SymbolNewsController_Phone.h"

#import "Symbol.h"

@implementation SymbolNewsView
@synthesize symbol = _symbol;
@synthesize viewController = _viewController;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
    if (self = [super initWithFrame:frame]) {
        _symbol = nil;
		
		_symbolNewsController = [[SymbolNewsController_Phone alloc] initWithManagedObjectContext:managedObjectContext];
		[self addSubview:_symbolNewsController.view];
		
		super.padding = 6.0f;
		super.cornerRadius = 10.0f;
		super.strokeWidth = 0.75f;
		
		CGFloat leftPadding = floorf(super.padding + super.strokeWidth * 2.0 + kBlur);
		CGFloat maxWidth = self.bounds.size.width - (leftPadding * 2.0f);
		CGFloat maxHeight = self.bounds.size.height - (leftPadding * 2.0f);
		
		CGRect tableFrame = CGRectMake(leftPadding, leftPadding, maxWidth, maxHeight);
		
		_symbolNewsController.view.frame = tableFrame;
		
		UIButton *newsButton = [[UIButton alloc] initWithFrame:self.bounds];
		[newsButton addTarget:self.viewController action:@selector(news:) forControlEvents:UIControlEventTouchUpInside];
		
		[self addSubview:newsButton];
		[newsButton release];
		

    }
    return self;
}

- (void)setSymbol:(Symbol *)symbol {
	if (symbol == _symbol) {
		return;
	}
	[_symbol release];
	_symbol = [symbol retain];
	_symbolNewsController.symbol = _symbol;
	
}

- (void)dealloc {
	[_symbolNewsController release];
	
	[_symbol release];
    [super dealloc];
}

@end
