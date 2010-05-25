//
//  NetworkDiagnosticViewController.h
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 09.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//

@class Reachability;

@interface NetworkDiagnosticViewController : UITableViewController {
	Reachability *_reachability;	
	
	NSString *_server;
	NSString *_port;
	
	CGRect _frame;
	
	NSArray *_headers;
	NSArray *_interfaces;
	NSArray *_serverDetails;
	NSArray *_serverAddresses;
	NSArray *_reachabilityDetails;
}

@property (nonatomic, retain) Reachability *reachability;
@property (nonatomic, retain) NSString *server;
@property (nonatomic, retain) NSString *port;
@property (assign) CGRect frame;

- (id)initWithFrame:(CGRect)frame;
- (void)updateReachability:(Reachability *)reach;

@end