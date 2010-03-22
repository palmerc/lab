//
//  MyListNavigationContoller.h
//  mTrader
//
//  Created by Cameron Lowell Palmer on 06.02.10.
//  Copyright 2010 Infront AS. All rights reserved.
//


@class MyListViewController;

@interface MyListNavigationController : UINavigationController {
@private
	MyListViewController *_myListViewController;
}

@property (nonatomic, retain) MyListViewController *myListViewController;

- (id)initWithContentViewController:(UIViewController *)rootViewController;

@end
