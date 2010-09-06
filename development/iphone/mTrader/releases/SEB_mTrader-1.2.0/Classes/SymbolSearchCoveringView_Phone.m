//
//  SymbolSearchCoveringView_Phone.m
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.06.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

#define DEBUG 0

#import "SymbolSearchCoveringView_Phone.h"


@implementation SymbolSearchCoveringView_Phone
@synthesize delegate;
@synthesize touchesEnabled;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		touchesEnabled = YES;
    }
    return self;
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
	if (touchesEnabled) {
		return self;
	} else {
		return nil;
	}
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent*)event {
	if (self.delegate && [self.delegate respondsToSelector:@selector(coveringView:didReceiveTouches:withEvent:)]) {
		[self.delegate coveringView:self didReceiveTouches:touches withEvent:event];
	}
}

// Very helpful when things seem not to be working.
#if DEBUG
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"Queried about %@", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
#endif

@end
