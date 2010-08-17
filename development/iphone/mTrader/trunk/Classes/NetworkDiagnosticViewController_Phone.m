//
//  NetworkDiagnosticViewController_Phone.m
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 09.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//

#import "NetworkDiagnosticViewController_Phone.h"

#import "Monitor.h"
#import "Reachability.h"

@implementation NetworkDiagnosticViewController_Phone

#pragma mark -
#pragma mark Initialization

- (id)init {
	self = [super initWithStyle:UITableViewStyleGrouped];
	if (self != nil) {
		self.title = NSLocalizedString(@"netDiagnostics", @"Network Diagnostics Page");
		
		NSString *yourIPAddressesSection = NSLocalizedString(@"yourIPAddresses", @"Your IP Addresses");
		NSString *serverDetailsSection = NSLocalizedString(@"serverDetails", @"Server Details");
		NSString *serverAddressesSection = NSLocalizedString(@"serverAddresses", @"Server Addresses");
		NSString *reachabilitySection = NSLocalizedString(@"reachability", @"Reachability");
		NSString *bytesSection = NSLocalizedString(@"bytes", @"Bytes");
		
		_headers = [[NSArray arrayWithObjects:yourIPAddressesSection, serverDetailsSection, serverAddressesSection, reachabilitySection, bytesSection, nil] retain];
		_interfaces = nil;
		_serverDetails = nil;
		_serverAddresses = nil;
		_reachabilityDetails = nil;
		_bytesDetails = nil;
	}
	return self;
}

#pragma mark -
#pragma mark UIViewController Delegate Methods
- (void)viewDidLoad {
	[super viewDidLoad];
	
	[self updateTable];
}

#pragma mark -
#pragma mark UITableViewDataSource

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	static NSString *CellIdentifier = @"DiagnosticTableCell_Phone";
    
	UITableViewCell *cell = (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
	}
	
	cell.selectionStyle = UITableViewCellSelectionStyleNone;
	if (indexPath.section == 0) {
		[cell.textLabel setText:[_interfaces objectAtIndex:indexPath.row]];
	} else if (indexPath.section == 1) {
		[cell.textLabel setText:[_serverDetails objectAtIndex:indexPath.row]];
	} else if (indexPath.section == 2) {
		[cell.textLabel setText:[_serverAddresses objectAtIndex:indexPath.row]];
	} else if (indexPath.section == 3) {
		[cell.textLabel setText:[_reachabilityDetails objectAtIndex:indexPath.row]];
	} else if (indexPath.section == 4) {
		[cell.textLabel setText:[_bytesDetails objectAtIndex:indexPath.row]];
	}
		
	return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return [_headers count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	if (section == 0) {
		return [_interfaces count];
	} else if (section == 1) {
		return [_serverDetails count];	
	} else if (section == 2) {
		return [_serverAddresses count];
	} else if (section == 3) {
		return [_reachabilityDetails count];
	} else if (section == 4) {
		return [_bytesDetails count];
	} else {
		return 0;
	}
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	return [_headers objectAtIndex:section];
}

#pragma mark -
#pragma mark UITableViewController Delegate Methods

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	[tableView deselectRowAtIndexPath:indexPath animated:NO];
}

- (void)updateTable {
	Monitor *monitor = [Monitor sharedManager];

	_interfaces = [[monitor interfaces] retain];
	
	_serverAddresses = [[monitor serverAddresses] retain];
		
	NetworkStatus status = [monitor currentReachabilityStatus];

	if (_reachabilityDetails != nil) {
		[_reachabilityDetails release];
	}
	NSString *remoteReachabilityText;
	switch (status) {
		case NotReachable:
			remoteReachabilityText = NSLocalizedString(@"notReachable", @"Not Reachable");
			break;
		case ReachableViaWiFi:
			remoteReachabilityText = NSLocalizedString(@"wifiReachable", @"Reachable via WiFi");
			break;
		case ReachableViaWWAN:
			remoteReachabilityText = NSLocalizedString(@"wwanReachable", @"Reachable via WWAN");
			break;
		default:
			remoteReachabilityText = NSLocalizedString(@"undefinedReachable", @"Reachability undefined");
			break;
	}
	_reachabilityDetails = [[NSArray arrayWithObjects:remoteReachabilityText, nil] retain];
	
	NSString *host = monitor.mTraderHost;
	NSString *port = [[NSNumber numberWithInteger:monitor.mTraderPort] stringValue];
	_serverDetails = [[NSArray arrayWithObjects:host, port, nil] retain];
	
	NSString *bytesReceivedText = [NSString stringWithFormat:@"%@: %d", NSLocalizedString(@"bytesReceived", @"Bytes Received"), monitor.bytesReceived];
	NSString *bytesSentText = [NSString stringWithFormat:@"%@: %d", NSLocalizedString(@"bytesSent", @"Bytes Sent"), monitor.bytesSent];
	_bytesDetails = [[NSArray arrayWithObjects:bytesReceivedText, bytesSentText, nil] retain];
	
	[self.tableView reloadData];
}

#pragma mark -
#pragma mark Memory Management

- (void)dealloc {
	[_headers release];
	[_interfaces release];
	[_serverDetails release];
	[_serverAddresses release];
	[_reachabilityDetails release];
	[_bytesDetails release];
	
	[super dealloc];
}

@end
