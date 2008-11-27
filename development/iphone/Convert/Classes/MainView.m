#import "MainView.h"

@implementation MainView

@synthesize thePicker, pickerArray;

- (IBAction)updateToLeftSide:(id)sender {
	thePicker.hidden = NO;
    rightSideText.text = [NSString stringWithFormat:@"%.2\f", (9.0 / 5.0 * [leftSideText.text floatValue]) + 32];
}
- (IBAction)updateToRightSide:(id)sender {
	thePicker.hidden = YES;
    leftSideText.text = [NSString stringWithFormat:@"%.2f", (5.0 / 9.0) * ([rightSideText.text floatValue] - 32)];
}

- (void) dealloc
{
	[thePicker release];
	[pickerArray release];
	[super dealloc];
}


@end
