//
//  NetworkDiagnosticViewController_Phone.m
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 09.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//

#import "NetworkDiagnosticViewController_Phone.h"

#import "Reachability.h"
#import "CPHost.h"

#include <netdb.h>
#include <arpa/inet.h>

@implementation NetworkDiagnosticViewController_Phone
@synthesize reachability = _reachability;
@synthesize server = _server;
@synthesize port = _port;

#pragma mark -
#pragma mark Initialization

- (id)init {
	self = [super initWithStyle:UITableViewStyleGrouped];
	if (self != nil) {
		self.title = NSLocalizedString(@"netDiagnostics", @"Network Diagnostics Page");
		self.server = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerAddress"]];
		self.port = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerPort"]];
		_reachability = nil;
		
		NSString *yourIPAddressesSection = NSLocalizedString(@"yourIPAddresses", @"Your IP Addresses");
		NSString *serverDetailsSection = NSLocalizedString(@"serverDetails", @"Server Details");
		NSString *serverAddressesSection = NSLocalizedString(@"serverAddresses", @"Server Addresses");
		NSString *reachabilitySection = NSLocalizedString(@"reachability", @"Reachability");
		
		_headers = [[NSArray arrayWithObjects:yourIPAddressesSection, serverDetailsSection, serverAddressesSection, reachabilitySection, nil] retain];
		_interfaces = nil;
		_serverDetails = [[NSArray arrayWithObjects:self.server, self.port, nil] retain];
		_serverAddresses = nil;
		_reachabilityDetails = nil;
	}
	return self;
}

#pragma mark -
#pragma mark UIViewController Methods

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];

	self.reachability = [Reachability reachabilityWithHostName:self.server];
	[self.reachability startNotifer];
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	[self.reachability stopNotifer];
	self.reachability = nil;
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

#pragma mark -
#pragma mark Reachability Methods
- (void)reachabilityChanged:(NSNotification *)note {
	Reachability *currentReachability = [note object];
	NSParameterAssert([currentReachability isKindOfClass:[Reachability class]]);

	[self updateReachability:(Reachability *)currentReachability];
}

- (void)updateReachability:(Reachability *)reach {
	const char *hostname = [self.server cStringUsingEncoding:NSASCIIStringEncoding];
	struct hostent *remoteHostEnt = gethostbyname(hostname);
	char **list;
	if (remoteHostEnt != NULL) {
		list = remoteHostEnt->h_addr_list;	
		
		if (_serverAddresses != nil) {
			[_serverAddresses release];
		}
		NSMutableArray *addresses = [NSMutableArray array];
		for (int i = 0; i < sizeof(list) / sizeof(struct in_addr *); i++) {
			struct in_addr *ip = (struct in_addr *)list[i];
			inet_ntoa(*ip);
			NSString *ipAddress = [NSString stringWithCString:inet_ntoa(*ip) encoding:NSASCIIStringEncoding];
			[addresses addObject:ipAddress];
		}
		_serverAddresses = [(NSArray *)addresses retain];
	}
	NetworkStatus status = [reach currentReachabilityStatus];

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
	
	if (_interfaces != nil) {
		[_interfaces release];
	}
	NSMutableArray *interfaces = [NSMutableArray array];
	NSDictionary *interfacesToAddresses = [CPHost interfacesToAddresses];
	for (NSString *key in [interfacesToAddresses allKeys]) {
		[interfaces addObject:[NSString stringWithFormat:@"%@: %@", key, [interfacesToAddresses objectForKey:key]]];
	}
	_interfaces = [(NSArray *)interfaces retain];
	
	[self.tableView reloadData];
}

#pragma mark -
#pragma mark Memory Management

- (void)dealloc {
	[_reachability release];
	[_headers release];
	[_interfaces release];
	[_serverDetails release];
	[_serverAddresses release];
	[_reachabilityDetails release];
	[_server release];
	[_port release];
	
	[super dealloc];
}

@end
