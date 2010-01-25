//
//  NewsItemViewController.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 22.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "iTraderCommunicator.h"

@interface NewsItemViewController : UIViewController <NewsItemDataDelegate> {
	NSString *_newsItemID;
	IBOutlet UILabel *date;
	IBOutlet UILabel *time;
	IBOutlet UILabel *headline;
	IBOutlet UITextView *body;

	CGSize sizeOfLine;
}

@property (nonatomic, retain) NSString *newsItemID;
@property (nonatomic, retain) IBOutlet UILabel *date;
@property (nonatomic, retain) IBOutlet UILabel *time;
@property (nonatomic, retain) IBOutlet UILabel *headline;
@property (nonatomic, retain) IBOutlet UITextView *body;

- (id)initWithNewsItem:(NSString *)newsItemID;
- (NSString *)cleanString:(NSString *)string;

@end
