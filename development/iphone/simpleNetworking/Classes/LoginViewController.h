//
//  LoginViewController.h
//  simpleNetworking
//
//  Created by Cameron Lowell Palmer on 17.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface LoginViewController : UIViewController {
	IBOutlet UILabel *statusLabel;
	IBOutlet UITextField *userTextField;
	IBOutlet UITextField *passwordTextField;
}

- (IBAction)login:(id)sender;

@end
