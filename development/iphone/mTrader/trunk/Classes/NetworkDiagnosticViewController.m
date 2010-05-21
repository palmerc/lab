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
@synthesize remoteHost = _remoteHost;
@synthesize remoteIP = _remoteIP;
@synthesize remotePort = _remotePort;
@synthesize remoteReachability = _remoteReachability;
@synthesize connectionType = _connectionType;
@synthesize yourIPAddress = _yourIPAddress;
@synthesize loginStatus = _loginStatus;

#pragma mark -

- (id)init {
	self = [super init];
	if (self != nil) {
		self.server = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerAddress"]];
		self.port = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"mTraderServerPort"]];
		_reachability = nil;
	}
	return self;
}

- (id)initWithFrame:(CGRect)frame {
	self.frame = frame;
	return [self init];
}

- (void)loadView {
	UIView *aView = [[UIView alloc] initWithFrame:self.frame];
	aView.backgroundColor = [UIColor lightGrayColor];
	
	UIFont *labelFont = [UIFont systemFontOfSize:17.0f];
	NSString *stringToMeasure = @"X";
	CGSize textSize = [stringToMeasure sizeWithFont:labelFont];
	CGRect labelFrame = CGRectMake(5.0, 20.0, self.frame.size.width - 5.0, textSize.height);
	UIColor *labelTextColor = [UIColor blackColor];	
	UIColor *labelBackgroundColor = [UIColor whiteColor];
	
	_remoteHost = [[UILabel alloc] initWithFrame:labelFrame];
	_remoteHost.font = labelFont;
	_remoteHost.textColor = labelTextColor;
	_remoteHost.backgroundColor = labelBackgroundColor;
	_remoteHost.text = self.server;
	[aView addSubview:_remoteHost];
	[_remoteHost release];
	
	labelFrame.origin.y += textSize.height;
	_remoteIP = [[UILabel alloc] initWithFrame:labelFrame];
	_remoteIP.font = labelFont;
	_remoteIP.textColor = labelTextColor;
	_remoteIP.backgroundColor = labelBackgroundColor;
	[aView addSubview:_remoteIP];
	[_remoteIP release];
		
	labelFrame.origin.y += textSize.height;
	_remotePort = [[UILabel alloc] initWithFrame:labelFrame];
	_remotePort.font = labelFont;
	_remotePort.textColor = labelTextColor;
	_remotePort.backgroundColor = labelBackgroundColor;
	_remotePort.text = self.port;
	[aView addSubview:_remotePort];	
	[_remotePort release];
	
	labelFrame.origin.y += textSize.height;
	_yourIPAddress = [[UILabel alloc] initWithFrame:labelFrame];
	_yourIPAddress.font = labelFont;
	_yourIPAddress.textColor = labelTextColor;
	_yourIPAddress.backgroundColor = labelBackgroundColor;
	[aView addSubview:_yourIPAddress];	
	[_yourIPAddress release];
	
	labelFrame.origin.y += textSize.height;
	_remoteReachability = [[UILabel alloc] initWithFrame:labelFrame];
	_remoteReachability.font = labelFont;
	_remoteReachability.backgroundColor = [UIColor whiteColor];
	_remoteReachability.textColor = [UIColor blackColor];
	[aView addSubview:_remoteReachability];
	[_remoteReachability release];
	
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
- (void)reachabilityChanged:(NSNotification *)note {
	Reachability *currentReachability = [note object];
	NSParameterAssert([currentReachability isKindOfClass:[Reachability class]]);

	[self updateReachability:(Reachability *)currentReachability];
}


- (void)updateReachability:(Reachability *)reach {
	const char *hostname = [self.server cStringUsingEncoding:NSASCIIStringEncoding];
	struct hostent *remoteHostEnt = gethostbyname(hostname);
	char **list = remoteHostEnt->h_addr_list;

	NSMutableArray *addresses = [NSMutableArray array];
	for (int i = 0; i < sizeof(list) / sizeof(struct in_addr *); i++) {
		struct in_addr *ip = (struct in_addr *)list[i];
		inet_ntoa(*ip);
		NSString *ipAddress = [NSString stringWithCString:inet_ntoa(*ip) encoding:NSASCIIStringEncoding];
		[addresses addObject:ipAddress];
	}
	
	for (NSString *address in addresses) {	
		self.remoteIP.text = address;
	}
	NetworkStatus status = [reach currentReachabilityStatus];

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
			break;
	}
	self.remoteReachability.text = remoteReachabilityText;
		
	NSMutableArray *interfaces = [NSMutableArray array];
	NSDictionary *interfacesToAddresses = [CPHost interfacesToAddresses];
	for (NSString *key in [interfacesToAddresses allKeys]) {
		[interfaces addObject:[NSString stringWithFormat:@"%@: %@", key, [interfacesToAddresses objectForKey:key]]];
	}
	for (NSString *interface in interfaces) {
		self.yourIPAddress.text = interface;
	}
}

#pragma mark -
- (void)dealloc {
	[_remoteHost release];
	[_remoteIP release];
	[_remotePort release];
	[_remoteReachability release];
	[_yourIPAddress release];
	
	[_reachability release];
	[super dealloc];
}

@end
