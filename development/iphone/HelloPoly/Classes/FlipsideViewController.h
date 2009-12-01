//
//  FlipsideViewController.h
//  HelloPoly
//
//  Created by Cameron Lowell Palmer on 29.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

@protocol FlipsideViewControllerDelegate;

@interface FlipsideViewController : UIViewController {
	id <FlipsideViewControllerDelegate> delegate;
	
	IBOutlet UISegmentedControl *solidOrDashedSegmentedControl;
	
	int segmentedControlIndex;
}

@property int segmentedControlIndex;
@property (nonatomic, assign) id <FlipsideViewControllerDelegate> delegate;
- (IBAction)done;
- (IBAction)solidOrDashed:(id)sender;
@end


@protocol FlipsideViewControllerDelegate
- (void)flipsideViewControllerDidFinish:(FlipsideViewController *)controller;
- (void)setSolidOrDashed:(int)value;
@end
