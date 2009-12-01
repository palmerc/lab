//
//  Controller.h
//
//  Created by Cameron Lowell Palmer on 26.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "FlipsideViewController.h";

@class MainView;
@class GraphicsView;
@class PolygonShape;

@interface MainViewController : UIViewController <FlipsideViewControllerDelegate> {
	PolygonShape *polygon;
	int solidOrDashedIndex;
	
	IBOutlet GraphicsView *graphicsView;
	
	IBOutlet UILabel *numberOfSidesLabel;
	IBOutlet UILabel *degreesLabel;
	IBOutlet UILabel *radiansLabel;
	IBOutlet UILabel *minSidesLabel;
	IBOutlet UILabel *maxSidesLabel;
	
	IBOutlet UIButton *decreaseButton;
	IBOutlet UIButton *increaseButton;
	IBOutlet UIButton *flipItButton;
	
	IBOutlet UISlider *sidesSlider;
	
	IBOutlet UISegmentedControl *solidOrDashedSegmentedControl;
}

@property (nonatomic, retain) GraphicsView *graphicsView;
@property int solidOrDashedIndex;

- (IBAction)flipIt;
- (void)setNumberOfSides:(int)initial;
- (int)numberOfSides;
- (IBAction)increase:(id)sender;
- (IBAction)decrease:(id)sender;
- (IBAction)valueChanged:(id)sender;
- (void)updateView;
- (void)setSolidOrDashed:(int)value;

@end
