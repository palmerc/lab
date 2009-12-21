//
//  LoginViewController.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 12/17/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Communicator.h"

@interface LoginViewController : UIViewController <CommunicatorReceiveDelegate> {
	Communicator *comm;
	IBOutlet UITextField *usernameTextField;
	IBOutlet UITextField *passwordTextField;
	IBOutlet UILabel *statusLabel;
}

- (IBAction)login:(id)sender;

@end
