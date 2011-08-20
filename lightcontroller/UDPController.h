//
//  UDPController.h
//  lightcontroller
//
//  Created by gm on 20/08/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UDPController : NSObject

- (IBAction)colorChanged:(id)sender;
- (void)setColor:(NSColor*)thecolor;

@end

int s;
struct sockaddr_in si;
