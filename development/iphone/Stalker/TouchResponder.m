//
//  Stalker.m
//
//  Created by Cameron Lowell Palmer on 27.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "TouchResponder.h"

@implementation TouchResponder

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[UIView beginAnimations:@"stalk" context:nil];
	[UIView setAnimationDuration:1];
	[UIView setAnimationBeginsFromCurrentState:YES];
	UITouch *touch = [touches anyObject];
	stalker.center = [touch locationInView:self];
	[UIView commitAnimations];
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
}

@end
