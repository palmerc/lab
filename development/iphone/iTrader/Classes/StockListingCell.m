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

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        // Initialization code
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
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
