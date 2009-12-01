//
//  Controller.h
//
//  Created by Cameron Lowell Palmer on 19.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface Controller : UIViewController {
    IBOutlet UISlider *slider;
    IBOutlet UILabel *label;
}
- (IBAction)sliderChanged:(id)sender;
@end
