//
//  NetworkDiagnosticViewController_Phone.h
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 09.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//

@class Reachability;

@interface NetworkDiagnosticViewController_Phone : UITableViewController {
	Reachability *_reachability;	
	
	NSString *_server;
	NSString *_port;
		
	NSArray *_headers;
	NSArray *_interfaces;
	NSArray *_serverDetails;
	NSArray *_serverAddresses;
	NSArray *_reachabilityDetails;
}

@property (nonatomic, retain) Reachability *reachability;
@property (nonatomic, retain) NSString *server;
@property (nonatomic, retain) NSString *port;

- (void)updateReachability:(Reachability *)reach;

@end