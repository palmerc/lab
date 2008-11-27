#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface MainView : UIView <UIPickerViewDelegate> {
    IBOutlet UITextField *leftSideText;
    IBOutlet UITextField *rightSideText;
	UIPickerView *thePicker;
	NSArray *pickerArray;
	IBOutlet UILabel *diagLabel;
}

@property (nonatomic, retain) UIPickerView *thePicker;
@property (nonatomic, retain) NSArray *pickerArray;

- (IBAction)updateToLeftSide:(id)sender;
- (IBAction)updateToRightSide:(id)sender;

@end
