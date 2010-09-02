//
//  HalfRoundedRectangle.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 08.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@interface HalfRoundedRectangle : UIView {
    UIColor     *_strokeColor;
    UIColor     *_rectColor;
    CGFloat     _strokeWidth;
    CGFloat     _cornerRadius;
	CGFloat     _padding;
}
@property (nonatomic, retain) UIColor *strokeColor;
@property (nonatomic, retain) UIColor *rectColor;
@property CGFloat strokeWidth;
@property CGFloat cornerRadius;
@property CGFloat padding;

@end