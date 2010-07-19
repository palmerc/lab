//
//  StatusController.m
//  Popup
//
//  Created by Cameron Lowell Palmer on 07.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#import "StatusController.h"

#import "StatusView.h"

@interface StatusController ()
- (void)animationComplete;
@end



@implementation StatusController

@synthesize statusMessage = _statusMessage;

- (id)initWithFrame:(CGRect)frame {
	self = [super init];
	if (self != nil) {
		_frame = frame;

		_statusView = nil;
		_statusDisplayed = NO;
	}
	
	return self;
}

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	_statusView = [[StatusView alloc] initWithFrame:_frame];
	_statusView.rectColor = [UIColor lightGrayColor];
	_statusView.strokeColor = [UIColor whiteColor];
	
	self.view = _statusView;
	
	[_statusView release];
}

- (void)displayStatus {
	if (_statusDisplayed == YES) {
		return;
	}
	
	_statusDisplayed = YES;
	CGRect destinationFrame = _statusView.frame;
	destinationFrame.origin.y -= _statusView.bounds.size.height;
	
	[UIView beginAnimations:@"RiseDarthVader" context:NULL];
	[UIView setAnimationDuration:0.20f];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
	[UIView setAnimationBeginsFromCurrentState:YES];
	_statusView.frame = destinationFrame;
	
	[UIView commitAnimations];
}

- (void)hideStatus {
	if (_statusDisplayed == NO) {
		return;
	}
	
	_statusDisplayed = NO;
	CGRect destinationFrame = _statusView.frame;
	destinationFrame.origin.y += _statusView.bounds.size.height;
	
	[UIView beginAnimations:@"LowerDarthVader" context:NULL];
	[UIView setAnimationDuration:2.0f];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
	[UIView setAnimationBeginsFromCurrentState:YES];
	_statusView.frame = destinationFrame;
	
	[UIView commitAnimations];	
}

- (void)animationComplete {
	_statusDisplayed = YES;
}

- (void)setStatusMessage:(NSString *)statusMessage {
	_statusView.message = statusMessage;
}

- (void)dealloc {
	[_statusView release];
	
    [super dealloc];
}


@end
