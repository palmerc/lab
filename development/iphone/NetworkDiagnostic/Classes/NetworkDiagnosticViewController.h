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
	UILabel *_remoteHost;
	UILabel *_remoteIP;
	UILabel *_remotePort;
	UILabel *_remoteReachability;
	UILabel *_yourIPAddress;
}

@property (nonatomic, retain) Reachability *reachability;
@property (nonatomic, retain) NSString *server;
@property (nonatomic, retain) NSString *port;
@property (assign) CGRect frame;
@property (nonatomic, retain) UILabel *remoteHost;
@property (nonatomic, retain) UILabel *remoteIP;
@property (nonatomic, retain) UILabel *remotePort;
@property (nonatomic, retain) UILabel *remoteReachability;
@property (nonatomic, retain) UILabel *connectionType;
@property (nonatomic, retain) UILabel *yourIPAddress;
@property (nonatomic, retain) UILabel *loginStatus;

- (id)initWithFrame:(CGRect)frame;
- (void)updateReachability:(Reachability *)reach;

@end