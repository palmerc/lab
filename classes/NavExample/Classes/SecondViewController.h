//
//  SecondViewController.h
//  NavExample
//
//  Created by Cameron Lowell Palmer on 05.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SecondViewController : UIViewController {
	IBOutlet UILabel *label;
	NSString *text;
}

@property (retain) NSString *text;

- (id)initWithText:(NSString *)someText;

@end
