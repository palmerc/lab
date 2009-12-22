//
//  LoginViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright 2009 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
@class iTraderCommunicator;

@interface LoginViewController : UIViewController {
	iTraderCommunicator *iTrader;
	IBOutlet UITextField *usernameTextField;
	IBOutlet UITextField *passwordTextField;
	IBOutlet UIActivityIndicatorView *activityIndicator;
	IBOutlet UIButton *loginButton;
}

@property (nonatomic, retain) UITextField *usernameTextField;
@property (nonatomic, retain) UITextField *passwordTextField;
@property (nonatomic, retain) UIActivityIndicatorView *activityIndicator;
@property (nonatomic, retain) UIButton *loginButton;

- (IBAction)login:(id)sender;

@end
