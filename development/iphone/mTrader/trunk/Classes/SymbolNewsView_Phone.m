//
//  SymbolNewsView_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 18.08.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "SymbolNewsView_Phone.h"


@implementation SymbolNewsView_Phone
@synthesize headlineFont = _headlineFont;
@synthesize tableView = _tableView;
@synthesize newsAvailableLabel = _newsAvailableLabel;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		_headlineFont = [[UIFont boldSystemFontOfSize:14.0f] retain];
		
        _tableView = [[UITableView alloc] initWithFrame:CGRectZero];
		[self addSubview:_tableView];
		
		NSString *labelString = NSLocalizedString(@"noNewsAvailable", @"No News Available");
		
		_newsAvailableLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_newsAvailableLabel.textAlignment = UITextAlignmentCenter;
		_newsAvailableLabel.textColor = [UIColor blackColor];
		_newsAvailableLabel.backgroundColor = [UIColor whiteColor];
		_newsAvailableLabel.text = labelString;
		_newsAvailableLabel.hidden = YES;
		[_tableView addSubview:_newsAvailableLabel];		
    }
    return self;
}

- (void)layoutSubviews {
	CGRect viewBounds = self.bounds;
	
	CGSize labelSize = [@"X" sizeWithFont:_headlineFont];
	CGRect labelFrame = CGRectMake(viewBounds.origin.x, viewBounds.origin.y, viewBounds.size.width, labelSize.height);
	
	_tableView.frame = viewBounds;

	_newsAvailableLabel.font = _headlineFont;
	_newsAvailableLabel.frame = labelFrame;
}

- (void)dealloc {
	[_tableView release];
	[_newsAvailableLabel release];
	
	[_headlineFont release];
	
    [super dealloc];
}


@end
