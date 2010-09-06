//
//  StatusController.h
//  Popup
//
//  Created by Cameron Lowell Palmer on 07.07.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@class StatusView;

@interface StatusController : UIViewController {
@private
	CGRect _frame;
	NSString *_statusMessage;
	
	StatusView *_statusView;
	
	BOOL _statusDisplayed;
}

@property (nonatomic, retain) NSString *statusMessage;

- (void)displayStatus;
- (void)hideStatus;

@end
