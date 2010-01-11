//
//  iTraderCommunicator.m
//  iTraderCommunicator is a Singleton that the rest of the application can use to
//  communicate with the mTraderServer
//
//  Created by Cameron Lowell Palmer on 22.12.09.
//  Copyright 2009 Infront AS. All rights reserved.
//

#import "iTraderCommunicator.h"
#import "UserDefaults.h";
#import "Symbol.h"
#import "Feed.h"

@implementation iTraderCommunicator

static iTraderCommunicator *sharedCommunicator = nil;
@synthesize mTraderServerDataDelegate;
@synthesize stockAddDelegate;
@synthesize communicator = _communicator;
@synthesize defaults = _defaults;
@synthesize isLoggedIn = _isLoggedIn;

+ (iTraderCommunicator *)sharedManager {
	if (sharedCommunicator == nil) {
		sharedCommunicator = [[super allocWithZone:NULL] init];
	}
	return sharedCommunicator;
}

+ (id)allocWithZone:(NSZone *)zone {
	return [[self sharedManager] retain];
}

- (id)copyWithZone:(NSZone *)zone {
	return self;
}

- (id)retain {
	return self;
}

- (NSUInteger)retainCount {
	return NSUIntegerMax;
}

- (void)release {
	// do nothing
}

- (id)autorelease {
	return self;
}

- (id)init {
	self = [super init];
	if (self != nil) {
		_communicator = [[Communicator alloc] initWithSocket:@"wireless.theonlinetrader.com" port:7780];
		_communicator.delegate = self;
		_defaults = [UserDefaults sharedManager];
		_isLoggedIn = NO;
		_loginStatusHasChanged = NO;
	}
	return self;
}

- (void)dealloc {
	[self logout];
	[self.communicator stopConnection];
	[self.communicator release];
	[super dealloc];
}

// Delegate method from Communicator
- (void)dataReceived {
	UIApplication* app = [UIApplication sharedApplication]; 
	app.networkActivityIndicatorVisible = YES; 
	NSString *currentLine = [self.communicator readLine];
	NSLog(@"%@", currentLine);
	if ([currentLine rangeOfString:@"Request: login/OK"].location == 0) {
		_loginStatusHasChanged = YES;
		_isLoggedIn = YES;
	} else if ([currentLine rangeOfString:@"Request: login/failed.UsrPwd"].location == 0) {
		_loginStatusHasChanged = YES;
		_isLoggedIn = NO;
	} else if ([currentLine rangeOfString:@"Content-Length:"].location == 0) {
	
	} else if ([currentLine rangeOfString:@"Symbols:"].location == 0 || [currentLine rangeOfString:@"SecInfo:"].location == 0) {
		// remove the part of the string preceding the colon, and the rest of the symbols are colon separated.
		NSArray *rows = [self stripOffFirstElement:[currentLine componentsSeparatedByString:@":"]];
				
		// For each symbol
		for (NSString *row in rows) {
			Symbol *symbol = [[Symbol alloc] init];
			
			// The symbol data is separated by semi-colons.
			NSArray *columns = [self cleanStrings:[row componentsSeparatedByString:@";"]];
			
			// Clean up the white space.
			symbol.feedTicker = [columns objectAtIndex:0];
					
			// Split the feedNumberAndTicker into two components
			NSArray *feedNumberAndTicker = [symbol.feedTicker componentsSeparatedByString:@"/"];
			symbol.feedNumber = [feedNumberAndTicker objectAtIndex:0];
			NSString *tickerToo = [feedNumberAndTicker objectAtIndex:1];
			NSString *ticker = [columns objectAtIndex:1];
			// The ticker symbol from field 0 should match the same symbol in field 1
			assert([ticker isEqualToString:tickerToo]);
			symbol.tickerSymbol = ticker;
			
			symbol.name = [columns objectAtIndex:2];
			NSString *feedDescriptionAndCode = [columns objectAtIndex:3];
			symbol.type = [NSNumber numberWithInteger:[[columns objectAtIndex:4] integerValue]];
			symbol.orderbook = [columns objectAtIndex:5];
			symbol.isin = [columns objectAtIndex:6];
			symbol.exchangeCode = [columns objectAtIndex:7];
			
			Feed *feed = [[Feed alloc] init];
			feed.feedNumber = symbol.feedNumber;
			// Obj-C doesn't have regex so we will look from the end of the string to get the [xxx] feed code. 
			NSCharacterSet *leftSquareBracket = [NSCharacterSet characterSetWithCharactersInString:@"["]; 
			NSRange feedCodeRange = [feedDescriptionAndCode rangeOfCharacterFromSet:leftSquareBracket options:NSBackwardsSearch];
			NSInteger lengthOfFeedString = [feedDescriptionAndCode length];
			
			// Range compensating for the removal of square brackets
			NSRange codeRange;
			codeRange.location = feedCodeRange.location + 1;
			codeRange.length = (lengthOfFeedString - feedCodeRange.location) - 1;
						
			NSRange descriptionRange;
			descriptionRange.location = 0;
			descriptionRange.length = lengthOfFeedString - codeRange.length - 2;
			
			NSString *feedDescription = [feedDescriptionAndCode substringWithRange:descriptionRange];
			NSString *feedCode = [feedDescriptionAndCode substringWithRange:codeRange];
			feed.feedDescription = feedDescription;
			feed.code = feedCode;
						
			if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(addSymbol: withFeed:)]) {
				[self.mTraderServerDataDelegate addSymbol:symbol withFeed:feed];
			}
			[symbol release];
			[feed release];
			symbol = nil;
			feed = nil;
		}
	} else if ([currentLine rangeOfString:@"Request: addSec/OK"].location == 0) {
		[stockAddDelegate addOK];
	} else if ([currentLine rangeOfString:@"Request: addSec/failed.AlreadyExists"].location == 0) {
		[stockAddDelegate addFailedAlreadyExists];
	} else if ([currentLine rangeOfString:@"Request: addSec/failed.NoSuchSec"].location == 0) {
		[stockAddDelegate addFailedNotFound];
	} else if ([currentLine rangeOfString:@"Quotes: "].location == 0) {
		NSArray *quotesAndTheRest = [self stripOffFirstElement:[currentLine componentsSeparatedByString:@":"]];
		NSString *theRest = [quotesAndTheRest objectAtIndex:0];
		NSArray *quotes = [theRest componentsSeparatedByString:@"|"];
		if (mTraderServerDataDelegate && [mTraderServerDataDelegate respondsToSelector:@selector(updateQuotes:)]) {
			[mTraderServerDataDelegate updateQuotes:quotes];
		}
	} else if ([currentLine rangeOfString:@"Exchanges:"].location == 0) {
	} else if ([currentLine rangeOfString:@"NewsFeeds:"].location == 0) {
	}

	app.networkActivityIndicatorVisible = NO;
}

- (BOOL)loginStatusHasChanged {
	BOOL result = NO;
	if (_loginStatusHasChanged) {
		result = YES;
		
		_loginStatusHasChanged = NO;
	}
	return result;
}

- (void)logout {
	NSString *ActionLogout = @"Action: Logout";
	NSArray *logoutArray = [NSArray arrayWithObjects:ActionLogout, nil];
	NSString *logoutString = [self arrayToFormattedString:logoutArray];
	[self.communicator writeString:logoutString];
}

- (void)login {
	NSString *username = self.defaults.username;
	NSString *password = self.defaults.password;
	
	NSString *version = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
	NSString *build = [NSString stringWithFormat:@"%@",[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"]];
	
	NSString *ActionLogin = @"Action: login";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@/%@", username, password];
	NSString *Platform = [NSString stringWithFormat:@"Platform: %@ %@", [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemVersion]];
	NSString *Client = @"Client: iTrader";
	NSString *Version = [NSString stringWithFormat:@"VerType: %@.%@", version, build];
	NSString *ConnectionType = @"ConnType: Socket";
	NSString *Streaming = @"Streaming: 1";
	NSString *QFields = @"QFields: l;cp;b;a;av;bv;c;h;lo;o;v";
	//NSString *QFields = @"QFields: l";
	
	NSArray *loginArray = [NSArray arrayWithObjects:ActionLogin, Authorization, Platform, Client, Version, ConnectionType, Streaming, QFields, nil];
	NSString *loginString = [self arrayToFormattedString:loginArray];
	
	if ([self.communicator isConnected]) {
		[self.communicator stopConnection];
		_isLoggedIn = NO;
	}
	
	[self.communicator startConnection];
	[self.communicator writeString:loginString];
}

- (void)addSecurity:(NSString *)tickerSymbol {
	NSString *username = self.defaults.username;
	
	NSString *ActionAddSec = @"Action: addSec";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *Search = [NSString stringWithFormat:@"Search: %@", tickerSymbol];
	NSString *MCode = [NSString stringWithFormat:@"mCode: %@", @"Oslo Stocks [OSS]"];
	
	NSArray *addSecurityArray = [NSArray arrayWithObjects:ActionAddSec, Authorization, Search, MCode, nil];
	NSString *addSecurityString = [self arrayToFormattedString:addSecurityArray];
	
	[self.communicator writeString:addSecurityString];
}

- (void)removeSecurity:(NSString *)feedTicker {
	NSString *username = self.defaults.username;
	
	NSString *ActionRemSec = @"Action: remSec";
	NSString *Authorization = [NSString stringWithFormat:@"Authorization: %@", username];
	NSString *SecOid = [NSString stringWithFormat:@"SecOid: %@", feedTicker];
	
	NSArray *removeSecurityArray = [NSArray arrayWithObjects:ActionRemSec, Authorization, SecOid, nil];
	NSString *removeSecurityString = [self arrayToFormattedString:removeSecurityArray];
	
	[self.communicator writeString:removeSecurityString];
}

- (NSString *)arrayToFormattedString:(NSArray *)arrayOfStrings {
	NSString *EOL = @"\r\n";
	
	NSMutableString *appendableText = [[NSMutableString alloc] init];
	for (NSString *string in arrayOfStrings) {
		NSString *current = [[NSString alloc] initWithFormat:@"%@%@", string, EOL];
		[appendableText appendString:current];
		[current release];
	}
	[appendableText appendString:EOL]; // A blank line indicates the end of the sending block
	NSString *immutableString = [NSString stringWithUTF8String:[appendableText UTF8String]];
	[appendableText release];
	
	return immutableString;
}

- (NSArray *)stripOffFirstElement:(NSArray *)array {
	NSRange rowsWithoutFirstString;
	rowsWithoutFirstString.location = 1;
	rowsWithoutFirstString.length = [array count] - 1;
	NSArray *rows = [array subarrayWithRange:rowsWithoutFirstString];
	
	return rows;
}

-(NSArray *)cleanStrings:(NSArray *)strings {
	NSCharacterSet *whitespaceAndNewline = [NSCharacterSet whitespaceAndNewlineCharacterSet];
	NSMutableArray *mutableProduct = [[NSMutableArray alloc] init];
	
	for (NSString *string in strings) {
		[mutableProduct addObject:[string stringByTrimmingCharactersInSet:whitespaceAndNewline]];
	}
		
	NSArray *finalProduct = [NSArray arrayWithArray:mutableProduct];
	[mutableProduct release];
	
	return finalProduct;
}

@end
