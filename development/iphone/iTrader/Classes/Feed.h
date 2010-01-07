//
//  Feed.h
//  iTrader
//
//  Created by Cameron Lowell Palmer on 04.01.10.
//  Copyright 2010 InFront AS. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Feed : NSObject {
	NSString *_number;
	NSString *_feedDescription;
	NSString *_code;
	
	NSMutableArray *_symbols;
}

@property (nonatomic,retain) NSString *number;
@property (nonatomic,retain) NSString *feedDescription;
@property (nonatomic,retain) NSString *code;
@property (nonatomic, retain) NSMutableArray *symbols;
@end
