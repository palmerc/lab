//
//  StockListingCell.m
//  iTrader
//
//  Created by Cameron Lowell Palmer on 05.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import "StockListingCell.h"


@implementation StockListingCell
@synthesize delegate = _delegate;
@synthesize tickerLabel, nameLabel, valueButton;

#pragma mark -
#pragma mark Lifecycle
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        // Initialization code
    }
    return self;
}

- (void)setEditing:(BOOL)editing animated:(BOOL)animate {
	if (editing == YES) {
		self.valueButton.hidden = YES;
	} else {
		self.valueButton.hidden = NO;
	}
	[super setEditing:editing animated:animate];
}

-(IBAction) touchedValueButton:(id)sender {
	if (self.delegate && [self.delegate respondsToSelector:@selector(touchedValueButton:)]) {
		[self.delegate touchedValueButton:(id)sender];
	}
}

- (void)dealloc {
    [super dealloc];
}


@end
