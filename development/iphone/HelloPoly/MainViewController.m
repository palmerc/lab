//
//  Controller.m
//
//  Created by Cameron Lowell Palmer on 26.11.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "MainViewController.h"
#import "MainView.h"
#import "GraphicsView.h"
#import "PolygonShape.h"


@implementation MainViewController

@synthesize graphicsView;
@synthesize solidOrDashedIndex;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        polygon = [[PolygonShape alloc] init];
		// Fix the frame around the info button to be bigger. Otherwise you can't hit it.
		CGRect newInfoButtonRect = CGRectMake(flipItButton.frame.origin.x-30,
											  flipItButton.frame.origin.y-30,
											  flipItButton.frame.size.width+60,
											  flipItButton.frame.size.height+60);
		
		[flipItButton setFrame:newInfoButtonRect];		
    }
    return self;
}

- (void)flipsideViewControllerDidFinish:(FlipsideViewController *)controller {
	[self dismissModalViewControllerAnimated:YES];
}

- (void)flipIt {
	FlipsideViewController *controller = [[FlipsideViewController alloc] initWithNibName:@"FlipsideView" bundle:nil];
	controller.segmentedControlIndex = solidOrDashedIndex;
	controller.delegate = self;

	controller.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
	[self presentModalViewController:controller animated:YES];

	[controller release];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)viewDidUnload {
}


- (void)dealloc {
    [super dealloc];
}

- (void)setNumberOfSides:(int)sides {
	polygon.numberOfSides = sides;
	[self updateView];
}

- (int)numberOfSides {
	return polygon.numberOfSides;
}

- (void)setSolidOrDashedIndex:(int)value {
	solidOrDashedIndex = value;
	[self updateView];
}

- (IBAction)increase:(id)sender {
	[polygon setNumberOfSides:polygon.numberOfSides + 1];
	[self updateView];
}

- (IBAction)decrease:(id)sender {
	[polygon setNumberOfSides:polygon.numberOfSides - 1];
	[self updateView];
}

- (IBAction)valueChanged:(id)sender {
	[polygon setNumberOfSides:sidesSlider.value];
	[self updateView];
}

- (void)updateView {
	BOOL dashed = NO;
	if (solidOrDashedIndex == 1) {
		dashed = YES;
	}
		
	[graphicsView updateShape:polygon.name numberOfSides:polygon.numberOfSides dashedStroke:dashed];
	NSString *numberOfSidesString = [[NSString alloc] initWithFormat:@"%d", polygon.numberOfSides];
	NSString *degreesString = [[NSString alloc] initWithFormat:@"%.0f", polygon.angleInDegrees];
	NSString *radiansString = [[NSString alloc] initWithFormat:@"%.3f", polygon.angleInRadians];
	NSString *minString = [[NSString alloc] initWithFormat:@"%d", polygon.minimumNumberOfSides];
	NSString *maxString = [[NSString alloc] initWithFormat:@"%d", polygon.maximumNumberOfSides];
	
	sidesSlider.value = polygon.numberOfSides;
	numberOfSidesLabel.text = numberOfSidesString;
	degreesLabel.text = degreesString;
	radiansLabel.text = radiansString;
	minSidesLabel.text = minString;
	maxSidesLabel.text = maxString;
	
	[numberOfSidesString release];
	[degreesString release];
	[radiansString release];
	[minString release];
	[maxString release];
}

- (void)setSolidOrDashed:(int)value {
	solidOrDashedIndex = value;
	[self updateView];
}

@end
