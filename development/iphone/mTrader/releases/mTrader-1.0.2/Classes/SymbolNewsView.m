//
//  SymbolNewsView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 23.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "SymbolNewsView.h"

#import "SymbolNewsController.h"

#import "Symbol.h"

@implementation SymbolNewsView
@synthesize symbol = _symbol;
@synthesize viewController = _viewController;

#pragma mark -
#pragma mark Initialization
- (id)initWithFrame:(CGRect)frame andManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
    if (self = [super initWithFrame:frame]) {
        _symbol = nil;
		
		symbolNewsController = [[SymbolNewsController alloc] initWithManagedObjectContext:managedObjectContext];
		[self addSubview:symbolNewsController.view];
    }
    return self;
}

#pragma mark -
#pragma mark UIView drawing
- (void)drawRect:(CGRect)rect {
    super.padding = 6.0f;
	super.cornerRadius = 10.0f;
	super.strokeWidth = 0.75f;
	
	CGFloat leftPadding = ceilf(super.padding + super.strokeWidth * 2.0 + kBlur);
	CGFloat maxWidth = rect.size.width - leftPadding * 2.0f;
	CGFloat maxHeight = rect.size.height - leftPadding * 2.0f;
	
	CGRect tableFrame = CGRectMake(leftPadding, leftPadding, maxWidth, maxHeight);
	
	symbolNewsController.view.frame = tableFrame;
	
	UIButton *newsButton = [[UIButton alloc] initWithFrame:rect];
	[newsButton addTarget:self.viewController action:@selector(news:) forControlEvents:UIControlEventTouchUpInside];
	
	[self addSubview:newsButton];
	[newsButton release];
	
	[super drawRect:rect];
}


- (void)dealloc {
	[_symbol release];
    [super dealloc];
}

@end
