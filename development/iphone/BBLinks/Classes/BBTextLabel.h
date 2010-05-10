//
//  BBTextLabel.h
//  BBLinks
//
//  Created by Cameron Lowell Palmer on 10.05.10.
//  Copyright 2010 Bird and Bear Productions. All rights reserved.
//

@class BBStyledText;
@class BBStyledElement;
@class BBStyledBoxFrame;

@interface BBTextLabel : UIView {
	BBStyledText *_text;
	UIColor *_textColor;
	UIColor *_highlightedTextColor;
	UIFont *_font;
	UITextAlignment _textAlignment;
	
	UIEdgeInsets _contentInset;
	
	BOOL _highlighted;
	BBStyledElement *_highlightedNode;
	BBStyledBoxFrame *_highlightedFrame;
}

@property (nonatomic, retain) BBStyledText *text;
@property (nonatomic, retain) UIColor *textColor;
@property (nonatomic, retain) UIColor *highlightedTextColor;
@property (nonatomic, retain) UIFont *font;
@property (nonatomic) UITextAlignment textAlignment;
@property (nonatomic) UIEdgeInsets contentInset;
@property (nonatomic) BOOL highlighted;
@property (nonatomic, retain) BBStyledElement *highlightedNode;

@end
