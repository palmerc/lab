//
//  CPView.m
//  ResizeElement
//
//  Created by Cameron Lowell Palmer on 27.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


#import "CPView.h"

@implementation CPView
@synthesize topBar = _topBar;
@synthesize bottomBar = _bottomBar;
@synthesize helloLabel = _helloLabel;
@synthesize shrunk = _shrunk;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		_topBar = [[UIView alloc] initWithFrame:CGRectZero];
		_topBar.backgroundColor = [UIColor yellowColor];
		[self addSubview:_topBar];
		
		_helloLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		_helloLabel.textAlignment = UITextAlignmentLeft;
		_helloLabel.backgroundColor = [UIColor groupTableViewBackgroundColor];
		[self addSubview:_helloLabel];
		
		_bottomBar = [[UIView alloc] initWithFrame:CGRectZero];
		_bottomBar.backgroundColor = [UIColor redColor];
		[self addSubview:_bottomBar];
		
		_location = 0.0f;
		_shrunk = NO;
	}
    return self;
}

- (void)layoutSubviews {
	CGPoint center = self.center;
	CGRect topBarFrame = self.bounds;
	topBarFrame.size.height = 50.0f;
	_topBar.frame = topBarFrame;
	
	CGRect helloLabelFrame = self.bounds;
	CGSize helloLabelSize = [_helloLabel.text sizeWithFont:_helloLabel.font];
	helloLabelFrame.size.width = helloLabelSize.width;
	helloLabelFrame.size.height = helloLabelSize.height;
	_helloLabel.frame = helloLabelFrame;
	_helloLabel.center = center;
	
	CGRect centeredHelloLabelFrame = _helloLabel.frame;
	centeredHelloLabelFrame.origin.x = floorf(centeredHelloLabelFrame.origin.x);
	centeredHelloLabelFrame.origin.y = floorf(centeredHelloLabelFrame.origin.y);
	_helloLabel.frame = centeredHelloLabelFrame;
	
	CGRect bottomBarFrame = self.bounds;
	bottomBarFrame.size.height = 50.0f;
	
	if (_shrunk) {
		bottomBarFrame.origin.y = self.bounds.size.height;
	} else {
		bottomBarFrame.origin.y = self.bounds.size.height - 50.0f;
	}
	
	_bottomBar.frame = bottomBarFrame;
}

- (void)shrink {
	if (_shrunk) {
		return;
	}
	
	_shrunk = YES;
	
	CGRect oldFrame = _bottomBar.frame;
	CGRect newFrame = oldFrame;
	newFrame.origin.y += 50.0f;
	
	[UIView beginAnimations:@"Shrinking Violet" context:nil];
	[UIView setAnimationBeginsFromCurrentState:YES];
	[UIView setAnimationCurve:UIViewAnimationCurveLinear];
	[UIView setAnimationDuration:5.0f];
	
	_bottomBar.frame = newFrame;
	[UIView commitAnimations];
}

- (void)stretch {
	if (!_shrunk) {
		return;
	}
	
	_shrunk = NO;
	
	CGRect oldFrame = _bottomBar.frame;
	CGRect newFrame = oldFrame;
	newFrame.origin.y -= 50.0f;
	
	[UIView beginAnimations:@"Sprouting Asparagus" context:nil];
	[UIView setAnimationBeginsFromCurrentState:YES];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	[UIView setAnimationDuration:2.0f];
	
	_bottomBar.frame = newFrame;
	[UIView commitAnimations];
}

- (void)dealloc {
	[_helloLabel release];
	[_topBar release];
	[_bottomBar release];
	
    [super dealloc];
}


@end
