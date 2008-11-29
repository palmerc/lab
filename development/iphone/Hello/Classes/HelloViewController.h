//
//  HelloViewController.h
//  Hello
//
//  Created by Cameron Palmer on 28/11/2008.
//  Copyright University of North Texas 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HelloViewController : UIViewController {
	IBOutlet UILabel *helloLabel;
}

- (IBAction)hello:(id)sender;

@end

