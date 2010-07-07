//
//  StatusView.h
//  Popup
//
//  Created by Cameron Lowell Palmer on 07.07.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


@interface StatusView : UIView {
@private
	UIActivityIndicatorView *_activityIndicator;
	
	NSString *_message;
	CGFloat fontSize;
	CGSize textSize;
}

@property (nonatomic, readonly) UIActivityIndicatorView *activityIndicator;
@property (nonatomic, retain) NSString *message;

@end
