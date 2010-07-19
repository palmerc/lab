//
//  NetworkDiagnosticViewController_Phone.h
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 09.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//


@interface NetworkDiagnosticViewController_Phone : UITableViewController {
@private
	NSArray *_headers;
	NSArray *_interfaces;
	NSArray *_serverDetails;
	NSArray *_serverAddresses;
	NSArray *_reachabilityDetails;
	NSArray *_bytesDetails;
}

- (void)updateTable;

@end