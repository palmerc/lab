//
//  Controller.m
//
//  Created by Cameron Lowell Palmer on 19.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "Controller.h"

@implementation Controller
- (IBAction)sliderChanged:(id)sender {
    label.text = [NSString stringWithFormat:@"%.1f", slider.value];
}
@end
