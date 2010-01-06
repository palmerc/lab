//
//  Feed.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Feed : NSObject {
	NSString *number;
	NSString *feedDescription;
	NSString *code;
}

@property (nonatomic,retain) NSString *number;
@property (nonatomic,retain) NSString *feedDescription;
@property (nonatomic,retain) NSString *code;

-(BOOL)isEqualToString:(NSString *)aString;
-(NSInteger)length;

@end
