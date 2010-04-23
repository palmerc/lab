//
//  GradientView.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 19.03.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "GradientView.h"

#import "GradientLayer.h"

@implementation GradientView

- (void)drawRect:(CGRect)rect {
	CGRect labelFrame = rect;
	
	UIColor *topLine = [UIColor colorWithRed:0.74 green:0.78 blue:0.83 alpha:1.0];
	UIColor *topOfFade = [UIColor colorWithRed:0.53 green:0.62 blue:0.71 alpha:1.0];
	UIColor *bottomOfFade = [UIColor colorWithRed:0.45 green:0.54 blue:0.65 alpha:1.0];
	UIColor *bottomLine = [UIColor colorWithRed:0.44 green:0.52 blue:0.64 alpha:1.0];

	NSArray *colors = [NSArray arrayWithObjects:(id)topLine.CGColor, 
					   (id)topOfFade.CGColor,
					   (id)bottomOfFade.CGColor,
					   (id)bottomLine.CGColor,
					   nil];
	NSArray *locations = [NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0],
						  [NSNumber numberWithFloat:0.45],
						  [NSNumber numberWithFloat:0.55],
						  [NSNumber numberWithFloat:1.0],
						  nil];
	CAGradientLayer *gradient = [CAGradientLayer layer];
	gradient.colors = colors;
	gradient.locations = locations;
	gradient.frame = labelFrame;
	[self.layer addSublayer:gradient];
	
	// Setup the text color
	UIColor *sectionTextColor = [UIColor colorWithWhite:1.0 alpha:1.0];
	UIColor *sectionTextShadowColor = [UIColor colorWithWhite:0.0 alpha:0.44];
	CGSize shadowOffset = CGSizeMake(0.0, 1.0);
	
	UILabel *aLabel = [[UILabel alloc] initWithFrame:labelFrame];
	aLabel.textColor = sectionTextColor;
	aLabel.shadowColor = sectionTextShadowColor;
	aLabel.shadowOffset = shadowOffset;
	aLabel.backgroundColor = [UIColor clearColor];	
	aLabel.textAlignment = UITextAlignmentCenter;
	[self addSubview:aLabel];
	[aLabel release];
	
	[super drawRect:rect];
}

@end
