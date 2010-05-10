//
//  BBTextLabel.m
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

#import "BBTextLabel.h"
#import "BBStyledText.h"

@implementation BBTextLabel

@synthesize text = _text;
@synthesize textColor = _textColor;
@synthesize highlightedTextColor = _highlightedTextColor;
@synthesize font = _font;
@synthesize textAlignment = _textAlignment;
@synthesize contentInset = _contentInset;
@synthesize highlighted = _highlighted;
@synthesize highlightedNode = _highlightedNode;

- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self != nil) {
		_textAlignment = UITextAlignmentLeft;
		_contentInset = UIEdgeInsetsZero;
		
		self.font = [UIFont systemFontOfSize:17.0];
		self.backgroundColor = [UIColor whiteColor];
		self.contentMode = UIViewContentModeRedraw;
	}
	return self;
}

- (BOOL)isHighlighted {
	return _highlighted;
}

- (void)setStyle:(BBStyle *)style forFrame:(BBStyledBoxFrame *)frame {
	if ([frame isKindOfClass:[TTStyledInlineFrame class]]) {
		BBStyledInlineFrame *inlineFrame = (BBStyledInlineFrame *)frame;
		while (inlineFrame.inlinePreviousFrame) {
			
		}
		
}

- (void)dealloc {
	[_text release];
	[_font release];
	[_textColor release];
	[_highlightedTextColor release];
	[_highlightedNode release];
	[_highlightedFrame release];
	
    [super dealloc];
}

@end
