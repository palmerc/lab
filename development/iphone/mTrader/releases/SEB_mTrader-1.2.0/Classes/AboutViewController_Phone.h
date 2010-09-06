//
//  AboutViewController_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 27.12.09.
//  Copyright 2009 InFront AS. All rights reserved.
//
@interface AboutViewController_Phone : UIViewController {
@private
	UILabel *versionTextField;
}

@property (nonatomic, retain) IBOutlet UILabel *versionTextField;

- (IBAction)buttonAction;

@end
