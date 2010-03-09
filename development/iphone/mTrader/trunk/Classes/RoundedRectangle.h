//
//  RoundedRectangle.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 08.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define kDefaultStrokeColor         [UIColor darkGrayColor]
#define kDefaultRectColor           [UIColor whiteColor]
#define kDefaultStrokeWidth         0.75
#define kDefaultCornerRadius        10.0

@interface RoundedRectangle : UIView {
    UIColor     *strokeColor;
    UIColor     *rectColor;
    CGFloat     strokeWidth;
    CGFloat     cornerRadius;
}
@property (nonatomic, retain) UIColor *strokeColor;
@property (nonatomic, retain) UIColor *rectColor;
@property CGFloat strokeWidth;
@property CGFloat cornerRadius;

@end