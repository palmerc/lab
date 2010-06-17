//
//  SymbolSearchCoveringView_Phone.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 16.06.10.
//  Copyright 2010 Infront AS. All rights reserved.
//

@protocol CoveringViewDelegate <NSObject>
@optional
- (void)coveringView:(id)sender didReceiveTouches:(NSSet *)touches withEvent:(UIEvent *)event;
@end


@interface SymbolSearchCoveringView_Phone : UIView {
@private
	id <CoveringViewDelegate> delegate;
	
	BOOL touchesEnabled;
}

@property (assign) id <CoveringViewDelegate> delegate;
@property BOOL touchesEnabled;

@end
