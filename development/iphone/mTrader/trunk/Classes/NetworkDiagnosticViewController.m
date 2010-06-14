//
//  NetworkDiagnosticViewController.m
//  NetworkDiagnostic
//
//  Created by Cameron Lowell Palmer on 09.04.10.
//  Copyright Infront AS 2010. All rights reserved.
//

#import "NetworkDiagnosticViewController.h"

#import "Reachability.h"
#import "CPHost.h"

#include <netdb.h>
#include <arpa/inet.h>

@implementation NetworkDiagnosticViewController
@synthesize reachability = _reachability;
@synthesize server = _server;
@synthesize port = _port;
@synthesize frame = _frame;

#pragma mark -

- (id)init {
	self = [super init];
	if (self != nil) {
		self.title = NSLocalizedString(@"netDiagnostics", @"Network Diagnostics Page");
		self.server = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerAddress"]];
		self.port = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerPort"]];
		_reachability = nil;
		
		_headers = [[NSArray arrayWithObjects:@"Your IP Addresses", @"Server Details", @"Server Addresses", @"Reachability", nil] retain];
		_interfaces = nil;
		_serverDetails = [[NSArray arrayWithObjects:self.server, self.port, nil] retain];
		_serverAddresses = nil;
		_reachabilityDetails = nil;
	}
	return self;
}

- (id)initWithFrame:(CGRect)frame {
	self.frame = frame;
	return [self init];
}

- (void)loadView {
	UITableView *aView = [[UITableView alloc] initWithFrame:self.frame style:UITableViewStyleGrouped];
	aView.backgroundColor = [UIColor lightGrayColor];
	aView.delegate = self;
	aView.dataSource = self;
	
	self.view = aView;
	[aView release];
}


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
	static NSString *CellIdentifier = @"DiagnosticTableCell";
    
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

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	[tableView deselectRowAtIndexPath:indexPath animated:NO];
}

#pragma mark -
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
			remoteReachabilityText = @"Not Reachable";
			break;
		case ReachableViaWiFi:
			remoteReachabilityText = @"Reachable via WiFi";
			break;
		case ReachableViaWWAN:
			remoteReachabilityText = @"Reachable via WWAN";
			break;
		default:
			remoteReachabilityText = @"Reachability undefined";
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
