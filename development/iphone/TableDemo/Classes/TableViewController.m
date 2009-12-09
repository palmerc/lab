//
//  TableViewController.m
//  TableDemo
//
//  Created by Cameron Lowell Palmer on 07.12.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "TableViewController.h"


@implementation TableViewController

- (id)initWithStyle:(UITableViewStyle)style
{
	self = [super initWithStyle:style];
	if (self != nil) {
		// Setup
		NSCharacterSet *forwardSlash = [NSCharacterSet characterSetWithCharactersInString:@"/"];
		NSArray *timeZonesArray = [NSTimeZone knownTimeZoneNames];
				
		NSMutableSet *regionsSet = [[NSMutableSet alloc] init];
		timeZoneDictionary = [[NSMutableDictionary alloc] init];
		
		// For each time zone entry
		for (NSString *timeZone in timeZonesArray)
		{
			NSArray *timeZonePieces = [timeZone componentsSeparatedByCharactersInSet:forwardSlash];			
			NSString *key = [timeZonePieces objectAtIndex:0];
			[regionsSet addObject:key];
			
			NSRange range;
			range.location = 1;
			range.length = [timeZonePieces count] - 1;
			
			NSArray *timeZonePiecesMinusKey = [timeZonePieces subarrayWithRange:range];
			NSMutableString *timeZoneStringMinusKey = [[NSMutableString alloc] init];
			for (NSString *subString in timeZonePiecesMinusKey) {
				[timeZoneStringMinusKey appendString:@"/"];
				[timeZoneStringMinusKey appendString:subString];
			}
			
			NSMutableArray *localArray = [timeZoneDictionary objectForKey:key];
			if (localArray == nil) {
				NSMutableArray *newArray = [[NSMutableArray alloc] init];
				[newArray addObject:timeZoneStringMinusKey];
				[timeZoneDictionary setObject:newArray forKey:key];
			} else {
				[localArray addObject:timeZoneStringMinusKey];
			}
			[timeZoneStringMinusKey release];
		}
		
		for (NSString *region in timeZoneDictionary) {
			NSMutableArray *locale = [timeZoneDictionary objectForKey:region];
			[locale sortUsingSelector:@selector(compare:)];
		}
		
		regionsArray = [[NSMutableArray alloc] initWithArray:[regionsSet allObjects]];
		[regionsArray sortUsingSelector:@selector(compare:)];
		
		[regionsSet release];
	}
	
	return self;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return [regionsArray objectAtIndex:section];
}

- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView 
{
	return regionsArray;
}	

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return [regionsArray count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	NSString *sectionName = [regionsArray objectAtIndex:section];
	return [[timeZoneDictionary objectForKey:sectionName] count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"TimeZoneCell"];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"TimeZoneCell"] autorelease];
	}
	NSString *sectionName = [regionsArray objectAtIndex:indexPath.section];
	
	NSArray *timeZone = [timeZoneDictionary objectForKey:sectionName];
	NSLog(@"%@", timeZone);
	NSString *text = [timeZone objectAtIndex:indexPath.row];
	text = [text stringByReplacingOccurrencesOfString:@"_" withString:@" "];
	[cell.textLabel setText:text];

	return cell;
	
}
	 
- (void)dealloc
{
	[timeZoneDictionary release];
	[regionsArray release];
    [super dealloc];
}


@end
