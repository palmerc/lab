//
//  CPView.h
//  ResizeElement
//
//  Created by Cameron Lowell Palmer on 27.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//



@interface CPView : UIView {
@private
	UIView *_topBar;
	UIView *_bottomBar;
	UILabel *_helloLabel;
	CGFloat _location;
	BOOL _shrunk;
}

@property (nonatomic, retain) UIView *topBar;
@property (nonatomic, retain) UIView *bottomBar;
@property (nonatomic, retain) UILabel *helloLabel;
@property (nonatomic, readonly, getter=isShrunk) BOOL shrunk;

- (void)shrink;
- (void)stretch;

@end
