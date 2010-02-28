//
//  XMLParser.m
//  LineGraph
//
//  Created by Cameron Lowell Palmer on 28.02.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


#import "XMLParser.h"

#import "DataPoint.h"

@implementation XMLParser
@synthesize points;

- (id)init {
	self = [super init];
	if (self != nil) {
		points = nil;
	}
	return self;
}


- (void)parserDidStartDocument:(NSXMLParser *)parser {
	if (points == nil) {
		points = [[NSMutableArray alloc] init];
	}
}

- (void)parserDidEndDocument:(NSXMLParser *)parser {
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName attributes:(NSDictionary *)attributeDict {
	// timestamp="1240272000" close="4501.6299" high="4542.9199" low="4390.7002" open="4488.6299" volume="43229700"
	if ([elementName isEqualToString:@"point"]) {
		DataPoint *point = [[DataPoint alloc] init];
		point.timestamp = [NSDate dateWithTimeIntervalSince1970:[[attributeDict valueForKey:@"timestamp"] integerValue]];
		point.close = [NSNumber numberWithDouble:[[attributeDict valueForKey:@"close"] doubleValue]];
		point.high = [NSNumber numberWithDouble:[[attributeDict valueForKey:@"high"] doubleValue]];
		point.low = [NSNumber numberWithDouble:[[attributeDict valueForKey:@"low"] doubleValue]];
		point.open = [NSNumber numberWithDouble:[[attributeDict valueForKey:@"open"] doubleValue]];
		point.volume = [NSNumber numberWithInteger:[[attributeDict valueForKey:@"volume"] integerValue]];
		[points addObject:point];
		[point release];
	}
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName {
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string {
	//NSLog(@"Characters: %@", string);
}

#pragma mark -
#pragma mark Debugging methods
/*
// Very helpful debug when things seem not to be working.
- (BOOL)respondsToSelector:(SEL)sel {
	NSLog(@"XMLParser queried about %@", NSStringFromSelector(sel));
	return [super respondsToSelector:sel];
}
*/
 
#pragma mark -
#pragma mark Memory management
- (void)dealloc {
	[points release];
	
    [super dealloc];
}


@end
